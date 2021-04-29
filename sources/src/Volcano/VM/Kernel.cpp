//
//
#include <chrono>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Kernel.hpp>

VOLCANO_VM_BEGIN

using namespace std::chrono;

static const char *initPath = "/init.lua";

Kernel::Kernel(uv_loop_t *loop):
	m_loop(loop)
{
	VOLCANO_ASSERT(m_loop != nullptr);

	uv_async_init(m_loop, &m_trapAsync, [] (uv_async_t *p) {
		auto kernel = reinterpret_cast<Kernel *>(p->data);
		kernel->handleTraps();
	});
	m_trapAsync.data = this;
}

Kernel::~Kernel(void)
{
	if (m_thread.joinable())
		stop();

	uvSyncClose(&m_trapAsync);
	uvSyncClose(&m_quitAsync);
}

bool Kernel::start(Traps *traps)
{
	VOLCANO_ASSERT(m_loop != nullptr);
	VOLCANO_ASSERT(traps != nullptr);

	m_traps = traps;

	std::promise<bool> initPromise;
	auto initResult = initPromise.get_future();
	m_thread = std::thread(&Kernel::threadMain, this, &initPromise);
	auto state = initResult.wait_for(15s);

	if (state != std::future_status::ready)
		return false;

	return initResult.get();
}

void Kernel::stop(void)
{
	if (m_thread.joinable()) {
		uv_async_send(&m_quitAsync);
		m_thread.join();
	}
}

void Kernel::postEvent(const SDL_Event &event)
{
	m_eventQueue[m_eventLast & EVENT_QUEUE_MASK] = event;
	m_eventLast += 1;
}

uv_loop_t *Kernel::loop(void)
{
	return m_loop;
}

void Kernel::addTaskHook(lua_State *L, lua_State *T)
{
	auto main = taskFromLua(L);
	VOLCANO_ASSERT(main != nullptr);

	auto task = taskFromLua(T);
	VOLCANO_ASSERT(task != nullptr);

	auto kernel = fromTask(task);
	VOLCANO_ASSERT(kernel != nullptr);

	task->data = kernel;
	task->loop = main->loop;

	uv_timer_init(task->loop, &task->sleepTimer);
	task->sleepTimer.data = task;

	cx_list_node_reset(&task->node);
	cx_list_append(&kernel->m_taskListReady, &task->node);
}

void Kernel::removeTaskHook(lua_State *L, lua_State *T)
{
	auto task = taskFromLua(T);
	VOLCANO_ASSERT(task != nullptr);

	uvSyncClose(&task->sleepTimer);
	cx_list_node_unlink(&task->node);
}

void Kernel::resumeTaskHook(lua_State *T, int n)
{
}

void Kernel::yieldTaskHook(lua_State *T, int n)
{
}

void Kernel::threadMain(std::promise<bool> *initPromise)
{
	VOLCANO_ASSERT(initPromise != nullptr);

	std::lock_guard<std::mutex> locker(m_mutex);

	uv_loop_t loop;

	if (uv_loop_init(&loop) < 0) {
		VOLCANO_LOGE("Failed to init vm loop.");
		initPromise->set_value(false);
		return;
	}

	ScopeGuard uvGuard([&] { uv_loop_close(&loop); });

	uv_async_init(&loop, &m_quitAsync, [] (uv_async_t *p) { uv_stop(p->loop); });
	ScopeGuard quitGuard([this] { uvSyncClose(&m_quitAsync); });

	uv_timer_t frameTimer;
	uv_timer_init(&loop, &frameTimer);
	uv_timer_start(&frameTimer, [] (uv_timer_t *p) {
		auto kernel = reinterpret_cast<Kernel *>(p->data);
		uint64_t curr = uv_now(p->loop);
		uint64_t pass = curr - kernel->m_timeLast;
		kernel->frame(float(pass));
		kernel->m_timeLast = curr;
		uv_update_time(p->loop);
	}, 0, 16);
	frameTimer.data = this;

	ScopeGuard updateTimerGuard([&frameTimer] {
		uv_timer_stop(&frameTimer);
		uvSyncClose(&frameTimer);
	});

	lua_State *L = luaL_newstate();
	if (L == nullptr) {
		VOLCANO_LOGE("Failed to create lua state.");
		initPromise->set_value(false);
		return;
	}

	ScopeGuard luaGuard([=] { lua_close(L); });

	auto task = taskFromLua(L);
	task->data = this;
	task->loop = &loop;

	cx_list_reset(&m_taskListReady);
	cx_list_reset(&m_taskListTrap);

	lua_pushcfunction(L, [] (lua_State *L) {
		auto loop = reinterpret_cast<uv_loop_t *>(lua_touserdata(L, 1));
		auto initResult = reinterpret_cast<std::promise<bool> *>(lua_touserdata(L, 2));
		fromLua(L)->protectedMain(L, loop, initResult);
		return 0;
	});

	lua_pushlightuserdata(L, &loop);
	lua_pushlightuserdata(L, initPromise);

	int ret = lua_pcall(L, 2, 0, 0);
	if (ret != LUA_OK) {
		VOLCANO_LOGE("Failed to enter VM protected mode [%d]: %s", ret, lua_tostring(L, -1));
		initPromise->set_value(false);
	}
}

void Kernel::protectedMain(lua_State *L, uv_loop_t *loop, std::promise<bool> *initPromise)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(loop != nullptr);
	VOLCANO_ASSERT(initPromise != nullptr);

	VOLCANO_LOGI("VM starting...");

	openLibs(L);

	if (!loadInitrc(L)) {
		VOLCANO_LOGE("Failed to load initrc.");
		initPromise->set_value(false);
		return;
	}

	uv_prepare_t schedulePrepare;
	uv_prepare_init(loop, &schedulePrepare);
	uv_prepare_start(&schedulePrepare, &Kernel::schedule);
	schedulePrepare.data = L;

	ScopeGuard schedulerGuard([&schedulePrepare] {
		uv_prepare_stop(&schedulePrepare);
		uvSyncClose(&schedulePrepare);
	});

	initPromise->set_value(true);

	VOLCANO_LOGI("VM main loop running...");
	uv_run(loop, UV_RUN_DEFAULT);
	VOLCANO_LOGI("VM main loop exited.");
}

void Kernel::openLibs(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	luaL_openlibs(L);

	luabridge::getGlobalNamespace(L)
		.beginNamespace("volcano")
			.addCFunction("current", &Kernel::sysCurrent)
			.addCFunction("task", &Kernel::sysTask)
			.addCFunction("sleep", &Kernel::sysSleep)
			.beginNamespace("window")
				//.addProperty("visible", )
				.beginNamespace("renderer")
				.endNamespace()
			.endNamespace()
			.beginNamespace("sound")
			.endNamespace()
		.endNamespace()
	.endNamespace();
}

bool Kernel::loadInitrc(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	VOLCANO_LOGI("Loading %s...", initPath);

	lua_State *T = lua_newthread(L);
	if (T == NULL) {
		VOLCANO_LOGE("Failed to create init task.");
		return false;
	}

	ScopeGuard taskGuard([L] { lua_pop(L, 1); });

	PHYSFS_File *fp = PHYSFS_openRead(initPath);
	if (fp == nullptr) {
		VOLCANO_LOGE("Failed to open '%s'.", initPath);
		return false;
	}

	ScopeGuard fpGuard([fp] { PHYSFS_close(fp); });

	auto size = PHYSFS_fileLength(fp);
	if (size < 1) {
		VOLCANO_LOGE("Empty file '%s'.", initPath);
		return false;
	}

	ByteArray buf(size);
	auto readSize = PHYSFS_readBytes(fp, buf.data(), size);
	if (readSize != size) {
		VOLCANO_LOGE("Failed to read '%s'.", initPath);
		return false;
	}

	luaL_loadbuffer(T, reinterpret_cast<const char *>(buf.data()), buf.size(), "init");

	return true;
}

void Kernel::frame(float elapsed)
{
	while (m_eventFirst < m_eventLast) {
		handleEvent(m_eventQueue[m_eventFirst & EVENT_QUEUE_MASK]);
		m_eventFirst += 1;
	}
}

void Kernel::schedule(uv_prepare_t *p)
{
	auto L = reinterpret_cast<lua_State *>(p->data);
	VOLCANO_ASSERT(L != nullptr);

	auto kernel = fromLua(L);
	VOLCANO_ASSERT(kernel != nullptr);

	cx_list_node_t *node;
	Task *task;
	lua_State *T;
	int ret;
	int nResults;

	while (!cx_list_is_empty(&kernel->m_taskListReady)) {
		node = cx_list_remove_head(&kernel->m_taskListReady);
		task = CX_MEMBEROF(node, Task, node);
		T = taskToLua(task);
		ret = lua_resume(T, L, 0, &nResults);
		if (ret != LUA_OK && ret != LUA_YIELD) {
			VOLCANO_LOGE("LUAERR: %d - %s", ret, lua_tostring(T, -1));
			// TODO vm exit?
		}
	}

	if (!cx_list_is_empty(&kernel->m_taskListTrap)) {
		uv_async_send(&kernel->m_trapAsync);
		kernel->m_cond.wait(kernel->m_mutex);
	}
}

void Kernel::handleEvent(const SDL_Event &event)
{
}

void Kernel::handleTraps(void)
{
	std::lock_guard<std::mutex> locker(m_mutex);

	while (!cx_list_is_empty(&m_taskListTrap)) {
		auto node = cx_list_remove_head(&m_taskListTrap);
		auto task = CX_MEMBEROF(node, Task, node);
		auto T = taskToLua(task);
		lua_call(T, 1, 1);
		task->trapResult = lua_tointeger(T, -1);
		cx_list_append(&m_taskListReady, &task->node);
	}

	m_cond.notify_one();
}

int Kernel::trapRequest(lua_State *T, lua_CFunction fn, lua_KFunction cb, lua_KContext ctx)
{
	VOLCANO_ASSERT(T != nullptr);
	VOLCANO_ASSERT(fn != nullptr);

	auto task = taskFromLua(T);
	auto kernel = fromTask(task);

	task->trapResult = 0;
	task->trapFunction = fn;
	task->trapCallback = cb;
	task->trapContext = ctx;

	cx_list_node_unlink(&task->node);
	cx_list_append(&kernel->m_taskListTrap, &task->node);

	return lua_yieldk(T, 0, lua_KContext(kernel), &Kernel::trapDone);
}

int Kernel::trapDone(lua_State *T, int status, lua_KContext ctx)
{
	return taskFromLua(T)->trapResult;
}

int Kernel::sysIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "version") == 0) {
		// TODO
	}
	else
		lua_pushvalue(L, 1);

	return 0;
}

int Kernel::sysNewIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "visible") == 0) {
	}
	else
		luaL_error(L, "unknown symbol 'volcano.%s'", name);

	return 0;
}


int Kernel::sysCurrent(lua_State *L)
{
	lua_pushthread(L);

	return 1;
}

int Kernel::sysTask(lua_State *L)
{
	int type;
	lua_State *T;

	type = lua_type(L, 1);
	switch (type) {
	case LUA_TFUNCTION:
		T = lua_newthread(L);
		if (T != NULL) {
			lua_pushvalue(L, 1);
			lua_xmove(L, T, 1);
		}
		else
			lua_pushnil(L);
		break;
	case LUA_TSTRING:
		T = lua_newthread(L);
		if (T != NULL)
			luaL_loadfile(T, lua_tostring(L, 1));
		else
			lua_pushnil(T);
		break;
	default:
		luaL_error(L, "unknown type.");
		break;
	}

	return 1;
}

int Kernel::sysSleep(lua_State *L)
{
	VOLCANO_LOGD(__func__);
	int ms = (int)luaL_checkinteger(L, 1);
	auto task = taskFromLua(L);
	Kernel *kernel = fromTask(task);

	if (ms > 0)
		uv_timer_start(&task->sleepTimer, &Kernel::sysSleepDone, ms, 0);
	else
		cx_list_append(&kernel->m_taskListReady, &task->node);

	return lua_yield(L, 0);
}

void Kernel::sysSleepDone(uv_timer_t *p)
{
	VOLCANO_LOGD(__func__);
	auto task = reinterpret_cast<Task *>(p->data);
	auto kernel = fromTask(task);

	cx_list_node_unlink(&task->node);
	cx_list_append(&kernel->m_taskListReady, &task->node);
}

int Kernel::sysWindowIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "visible") == 0) {
		// TODO
	}
	else if (strcmp(name, "resizable") == 0) {
		// TODO
	}
	else if (strcmp(name, "size") == 0) {
		// TODO
	}
	else if (strcmp(name, "fullscreen") == 0) {
		// TODO
	}
	else if (strcmp(name, "title") == 0) {
		// TODO
	}
	else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

	return 0;
}

int Kernel::sysWindowNewIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "visible") == 0) {
		bool v = !!luaL_checkinteger(L, 3);
		// TODO
	}
	else if (strcmp(name, "resizable") == 0) {
		bool v = !!luaL_checkinteger(L, 3);
		// TODO
	}
	else if (strcmp(name, "size") == 0) {
		// TODO
	}
	else if (strcmp(name, "fullscreen") == 0) {
		bool v = !!luaL_checkinteger(L, 3);
		// TODO
	}
	else if (strcmp(name, "title") == 0) {
		// TODO
	}
	else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

	return 0;
}

int Kernel::sysWindowRendererIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "clear") == 0) {
		// TODO
	}
	else if (strcmp(name, "clearColor") == 0) {
		// TODO
	}
	else if (strcmp(name, "drawTriangles") == 0) {
		// TODO
	}
	else if (strcmp(name, "drawNormals") == 0) {
		// TODO
	}
	else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

	return 0;
}

int Kernel::sysWindowRendererNewIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "clear") == 0) {
		bool v = !!luaL_checkinteger(L, 3);
		// TODO
	}
	else if (strcmp(name, "clearColor") == 0) {
		// TODO
	}
	else if (strcmp(name, "drawTriangles") == 0) {
		// TODO
	}
	else if (strcmp(name, "clearColor") == 0) {
		// TODO
	}
	else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

	return 0;
}

VOLCANO_VM_END

extern "C" void volcanoVMAddTaskHook(lua_State *L, lua_State * T)
{
	Volcano::VM::Kernel::fromLua(L)->addTaskHook(L, T);
}

extern "C" void volcanoVMRemoveTaskHook(lua_State *L, lua_State * T)
{
	Volcano::VM::Kernel::fromLua(L)->removeTaskHook(L, T);
}

extern "C" void volcanoVMResumeTaskHook(lua_State *T, int n)
{
	Volcano::VM::Kernel::fromLua(T)->resumeTaskHook(T, n);
}

extern "C" void volcanoVMYieldTaskHook(lua_State *T, int n)
{
	Volcano::VM::Kernel::fromLua(T)->resumeTaskHook(T, n);
}
