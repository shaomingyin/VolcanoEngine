//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Kernel.hpp>

VOLCANO_VM_BEGIN

VOLCANO_INLINE Task *taskFromLua(lua_State *L)
{
	return reinterpret_cast<Task *>(lua_getextraspace(L));
}

VOLCANO_INLINE lua_State *taskToLua(Task *task)
{
	return reinterpret_cast<lua_State *>(VOLCANO_PMOVB(task, LUA_EXTRASPACE));
}

VOLCANO_INLINE Kernel *kernelFromTask(Task *task)
{
	return reinterpret_cast<Kernel *>(task->data);
}

VOLCANO_INLINE Kernel *kernelFromLua(lua_State *L)
{
	return kernelFromTask(taskFromLua(L));
}

// Kernel

Kernel::Kernel(uv_loop_t *loop):
	m_hostLoop(loop)
{
	VOLCANO_ASSERT(m_hostLoop != nullptr);

	uv_async_init(m_hostLoop, &m_trapAsync, [](uv_async_t *p) {
		reinterpret_cast<Kernel *>(p->data)->trapHandler();
	});

	m_trapAsync.data = this;
}

Kernel::~Kernel(void)
{
}

bool Kernel::start(Traps *traps)
{
	VOLCANO_ASSERT(traps != nullptr);

	m_traps = traps;

	std::promise<bool> initResult;
	auto initFuture = initResult.get_future();

	m_threadRunning = true;
	m_thread = std::thread(&Kernel::threadMain, this, &initResult);

	return initFuture.get();
}

void Kernel::stop(void)
{
	VOLCANO_ASSERT(m_threadRunning);

	uv_async_send(&m_quitAsync);
	m_thread.join();
}

void Kernel::handleEvent(const SDL_Event &event)
{

}

void Kernel::addTaskHook(lua_State *L, lua_State *T)
{
	auto kernel = kernelFromLua(L);
	auto task = taskFromLua(T);
	task->data = taskFromLua(L)->data;
	uv_timer_init(&kernel->m_loop, &task->sleepTimer);
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

void Kernel::trapHandler(void)
{
	while (!volcanoListIsEmpty(&m_taskListTrap)) {
		auto node = volcanoListRemoveHead(&m_taskListTrap);
		auto task = VOLCANO_MEMBEROF(node, Task, node);
		auto T = taskToLua(task);
		lua_call(T, 1, 1);
		task->trapResult = lua_tointeger(T, -1);
		volcanoListAppend(&m_taskListReady, &task->node);
	}
}

void Kernel::threadMain(std::promise<bool> *initResult)
{
	std::lock_guard<std::mutex> locker(m_mutex);

	VOLCANO_ASSERT(initResult != nullptr);

	lua_State *L = luaL_newstate();
	if (L == nullptr) {
		VOLCANO_LOGE("Failed to create lua state.");
		initResult->set_value(false);
		return;
	}

	ScopeGuard luaGuard([=] { lua_close(L); });

	taskFromLua(L)->data = this;

	volcanoListReset(&m_taskListReady);
	volcanoListReset(&m_taskListTrap);

	lua_pushlightuserdata(L, initResult);
	lua_pushcfunction(L, &Kernel::luaMain);
	lua_pcall(L, 1, 0, 0);
}

int Kernel::luaMain(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	auto initResult = reinterpret_cast<std::promise<bool> *>(lua_touserdata(L, 1));
	VOLCANO_ASSERT(initResult != nullptr);

	kernelFromLua(L)->Main(L, initResult);

	return 0;
}

void Kernel::Main(lua_State *L, std::promise<bool> *initResult)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(initResult != nullptr);

	luaL_openlibs(L);
	initExports(L);

	if (!PHYSFS_init("volcano")) {
		VOLCANO_LOGE("Failed to init physfs.");
		initResult->set_value(false);
		return;
	}

	ScopeGuard physfsGuard([] { PHYSFS_deinit(); });

	if (!loadInitrc(L)) {
		VOLCANO_LOGE("Failed to load initrc.");
		initResult->set_value(false);
		return;
	}

	uv_loop_t loop;
	if (uv_loop_init(&loop) < 0) {
		VOLCANO_LOGE("Failed to create vm loop.");
		initResult->set_value(false);
		return;
	}

	ScopeGuard uvGuard([&] { uv_loop_close(&loop); });

	uv_async_init(&loop, &m_quitAsync, [](uv_async_t *p) { *reinterpret_cast<bool *>(p->data) = false; });
	m_quitAsync.data = &m_threadRunning;

	initResult->set_value(true);

	while (m_threadRunning) {
		schedule(L);
		uv_run(&loop, UV_RUN_ONCE);
	}
}

void Kernel::initExports(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	lua_newtable(L);

	// TODO

	lua_setglobal(L, "volcano");
}

bool Kernel::loadInitrc(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	PHYSFS_File *fp = PHYSFS_openAppend("/Init.lua");
	if (fp == nullptr) {
		VOLCANO_LOGE("Failed to open '/Init.lua'.");
		return false;
	}

	ScopeGuard fpGuard([=] { PHYSFS_close(fp); });

	auto size = PHYSFS_fileLength(fp);
	if (size < 1) {
		VOLCANO_LOGE("Empty file '/Init.lua'.");
		return false;
	}

	ByteArray buf(size);
	auto readSize = PHYSFS_readBytes(fp, buf.data(), size);
	if (readSize != size) {
		VOLCANO_LOGE("Failed to read '/Init.lua'.");
		return false;
	}

	// TODO load to lua.

	return true;
}

int Kernel::trap(lua_State *T)
{
	VOLCANO_ASSERT(T != nullptr);

	auto task = taskFromLua(T);

	task->trapResult = 0;
	volcanoListNodeUnlink(&task->node);
	volcanoListAppend(&m_taskListTrap, &task->node);

	uv_async_send(&m_trapAsync);
	m_cond.wait(m_mutex);

	return task->trapResult;
}

void Kernel::schedule(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	VolcanoListNode *node;
	Task *task;

	VOLCANO_LIST_FOREACH(node, &m_taskListReady) {
		task = VOLCANO_MEMBEROF(node, Task, node);
		// TODO
	}
}

VOLCANO_VM_END

extern "C" void volcanoVMAddTaskHook(lua_State *L, lua_State * T)
{
	Volcano::VM::kernelFromLua(L)->addTaskHook(L, T);
}

extern "C" void volcanoVMRemoveTaskHook(lua_State *L, lua_State * T)
{
	Volcano::VM::kernelFromLua(L)->removeTaskHook(L, T);
}

extern "C" void volcanoVMResumeTaskHook(lua_State *T, int n)
{
	Volcano::VM::kernelFromLua(T)->resumeTaskHook(T, n);
}

extern "C" void volcanoVMYieldTaskHook(lua_State *T, int n)
{
	Volcano::VM::kernelFromLua(T)->resumeTaskHook(T, n);
}
