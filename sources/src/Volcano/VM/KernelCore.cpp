//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Kernel.hpp>

VOLCANO_VM_BEGIN

static constexpr const char *initrcPath = "/init.lua";

Kernel::Kernel(uv_loop_t *loop, std::string_view rootPath):
	Base(loop, rootPath)
{
}

Kernel::~Kernel(void)
{
}

void Kernel::addTaskHook(lua_State *L, lua_State *T)
{
	auto main = taskFromLua(L);
	auto task = taskFromLua(T);
	auto kernel = fromTask(task);

	task->kernel = kernel;
	task->loop = main->loop;

	uv_timer_init(task->loop, &task->sleepTimer);
	task->sleepTimer.data = task;

	volcanoListNodeReset(&task->node);
	volcanoListAppend(&kernel->m_taskListReady, &task->node);
}

void Kernel::removeTaskHook(lua_State *L, lua_State *T)
{
	auto task = taskFromLua(T);

	volcanoListNodeUnlink(&task->node);
}

void Kernel::resumeTaskHook(lua_State *T, int n)
{
}

void Kernel::yieldTaskHook(lua_State *T, int n)
{
}

void Kernel::run(uv_loop_t *loop, std::promise<bool> *initPromise)
{
	VOLCANO_ASSERT(loop != nullptr);
	VOLCANO_ASSERT(initPromise != nullptr);

	lua_State *L = luaL_newstate();
	if (L == nullptr) {
		VOLCANO_LOGE("Failed to create lua state.");
		initPromise->set_value(false);
		return;
	}

	ScopeGuard luaGuard([=] { lua_close(L); });

	auto task = taskFromLua(L);
	task->kernel = this;
	task->loop = loop;

	volcanoListReset(&m_taskListReady);
	volcanoListReset(&m_taskListTrap);

	lua_pushcfunction(L, [](lua_State *L) {
		auto loop = reinterpret_cast<uv_loop_t *>(lua_touserdata(L, 1));
		auto initResult = reinterpret_cast<std::promise<bool> *>(lua_touserdata(L, 2));
		VOLCANO_ASSERT(initResult != nullptr);
		fromLua(L)->Main(L, loop, initResult);
		return 0;
	});

	lua_pushlightuserdata(L, loop);
	lua_pushlightuserdata(L, initPromise);

	int ret = lua_pcall(L, 2, 0, 0);
	if (ret != LUA_OK) {
		VOLCANO_LOGE("Failed to enter VM protected mode [%d].", ret);
		initPromise->set_value(false);
	}
}

void Kernel::frame(float elapsed)
{
	Base::frame(elapsed);
}

void Kernel::handleEvent(const SDL_Event &event)
{
	Base::handleEvent(event);
}

void Kernel::handleTrap(void)
{
	while (!volcanoListIsEmpty(&m_taskListTrap)) {
		auto node = volcanoListRemoveHead(&m_taskListTrap);
		auto task = VOLCANO_MEMBEROF(node, Task, node);
		auto T = taskToLua(task);
		lua_call(T, 1, 1);
		task->trapResult = lua_tointeger(T, -1);
		volcanoListAppend(&m_taskListReady, &task->node);
	}

	Base::handleTrap();
}

void Kernel::Main(lua_State *L, uv_loop_t *loop, std::promise<bool> *initPromise)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(loop != nullptr);
	VOLCANO_ASSERT(initPromise != nullptr);

	uv_prepare_t schedulePrepare;
	uv_prepare_init(loop, &schedulePrepare);
	uv_prepare_start(&schedulePrepare, &Kernel::schedule);
	schedulePrepare.data = L;

	ScopeGuard schedulerGuard([&] { uv_prepare_stop(&schedulePrepare); });

	luaL_openlibs(L);
	initExports(L);

	if (!loadInitrc(L)) {
		VOLCANO_LOGE("Failed to load initrc.");
		initPromise->set_value(false);
		return;
	}

	Base::run(loop, initPromise);
}

bool Kernel::loadInitrc(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	lua_State *T = lua_newthread(L);
	if (T == NULL) {
		VOLCANO_LOGE("Failed to create init task.");
		return false;
	}

	ScopeGuard taskGuard([=] { lua_pop(L, 1); });

	PHYSFS_File *fp = PHYSFS_openRead(initrcPath);
	if (fp == nullptr) {
		VOLCANO_LOGE("Failed to open '%s'.", initrcPath);
		return false;
	}

	ScopeGuard fpGuard([=] { PHYSFS_close(fp); });

	auto size = PHYSFS_fileLength(fp);
	if (size < 1) {
		VOLCANO_LOGE("Empty file '%s'.", initrcPath);
		return false;
	}

	ByteArray buf(size);
	auto readSize = PHYSFS_readBytes(fp, buf.data(), size);
	if (readSize != size) {
		VOLCANO_LOGE("Failed to read '%s'.", initrcPath);
		return false;
	}

	luaL_loadbuffer(T, reinterpret_cast<const char *>(buf.data()), buf.size(), "init");

	return true;
}

void Kernel::schedule(uv_prepare_t *p)
{
	auto L = reinterpret_cast<lua_State *>(p->data);
	VOLCANO_ASSERT(L != nullptr);

	auto kernel = fromLua(L);
	VOLCANO_ASSERT(kernel != nullptr);

	VolcanoListNode *node;
	Task *task;
	lua_State *T;
	int ret;
	int nResults;

	while (!volcanoListIsEmpty(&kernel->m_taskListReady)) {
		node = volcanoListRemoveHead(&kernel->m_taskListReady);
		task = VOLCANO_MEMBEROF(node, Task, node);
		T = taskToLua(task);
		ret = lua_resume(T, L, 0, &nResults);
		if (ret != LUA_OK && ret != LUA_YIELD) {
			VOLCANO_LOGE("LUAERR: %d - %s", ret, lua_tostring(T, -1));
			// TODO vm exit?
		}
	}

	if (!volcanoListIsEmpty(&kernel->m_taskListTrap))
		kernel->syncTrap();
}

int Kernel::trapRequest(lua_State *T)
{
	VOLCANO_ASSERT(T != nullptr);

	auto task = taskFromLua(T);
	auto kernel = fromTask(task);

	task->trapResult = 0;
	volcanoListNodeUnlink(&task->node);
	volcanoListAppend(&kernel->m_taskListTrap, &task->node);

	return lua_yieldk(T, 0, lua_KContext(kernel), &Kernel::trapDone);
}

int Kernel::trapDone(lua_State *T, int status, lua_KContext ctx)
{
	auto task = taskFromLua(T);
	auto kernel = reinterpret_cast<Kernel *>(ctx);

	// TODO

	return task->trapResult;
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
