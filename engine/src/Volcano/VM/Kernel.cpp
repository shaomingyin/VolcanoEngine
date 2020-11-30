//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Kernel.hpp>

VOLCANO_VM_BEGIN

Kernel::Kernel(uv_loop_t *loop):
    Core(loop)
{
    VOLCANO_ASSERT(loop != nullptr);

    uv_async_init(loop, &m_trapAsync, [](uv_async_t *async) {
        auto kernel = reinterpret_cast<Kernel *>(async->data);
        kernel->handleTraps();
    });

    m_trapAsync.data = this;
}

Kernel::~Kernel(void)
{
    uv_close_sync(&m_trapAsync);
}

void Kernel::handleTraps(void)
{
    VolcanoListNode *node;
    Task *task;
    bool handled = false;

    for (;;) {
        m_taskListMutex.lock();
        node = volcanoListRemoveHead(&m_taskListTrapped);
        m_taskListMutex.unlock();

        if (node != volcanoListKnot(&m_taskListTrapped)) {
            task = VOLCANO_MEMBEROF(node, Task, node);
            task->trapResult = task->trapFunc(taskToLua(task));
            handled = true;
        } else
            break;

        m_taskListMutex.lock();
        volcanoListAppend(&m_taskListReady, &task->node);
        m_taskListMutex.unlock();
    }

    if (handled)
        kick();
}

///////////////////////////////////////////////////////////////////////////////
// VM thread
///////////////////////////////////////////////////////////////////////////////

void Kernel::taskAdded(lua_State *L, lua_State *L1)
{
    Task *main = taskFromLua(L);
    Task *task = taskFromLua(L1);

    uv_timer_init(main->loop, &task->sleepTimer);

    task->loop = main->loop;
    task->data = main->data;
    task->sleepTimer.data = task;
    task->trapFunc = nullptr;
    task->trapResult = TRAP_ERROR_NOT_SET;

    volcanoListReset(&task->waitList);
    volcanoListNodeReset(&task->node);

    auto kernel = fromTask(task);
    kernel->m_taskListMutex.lock();
    volcanoListAppend(&kernel->m_taskListReady, &task->node);
    kernel->m_taskListMutex.unlock();
}

void Kernel::taskRemoved(lua_State *L, lua_State *L1)
{
    VOLCANO_UNUSED(L);

    Task *task = taskFromLua(L1);

    uv_close_sync(&task->sleepTimer);
    task->loop = nullptr;

    auto kernel = fromTask(task);
    kernel->m_taskListMutex.lock();
    volcanoListNodeUnlink(&task->node);
    kernel->m_taskListMutex.unlock();
}

void Kernel::taskResume(lua_State *L, int n)
{
    VOLCANO_UNUSED(L);
    VOLCANO_UNUSED(n);
}

void Kernel::taskYield(lua_State *L, int n)
{
    VOLCANO_UNUSED(L);
    VOLCANO_UNUSED(n);
}

void Kernel::run(uv_loop_t *loop, std::promise<bool> *initPromise)
{
    VOLCANO_ASSERT(loop != nullptr);
    VOLCANO_ASSERT(initPromise != nullptr);

    volcanoListReset(&m_taskListReady);
    volcanoListReset(&m_taskListTrapped);

    lua_State *L = luaL_newstate();
    if (L == nullptr) {
        initPromise->set_value(false);
        return;
    }

    lua_pushcfunction(L, [](lua_State *L) -> int {
        auto kernel = reinterpret_cast<Kernel *>(lua_touserdata(L, 1));
        auto loop = reinterpret_cast<uv_loop_t *>(lua_touserdata(L, 2));
        auto initPromise = reinterpret_cast<std::promise<bool> *>(lua_touserdata(L, 3));
        kernel->luaRun(L, loop, initPromise);
        return 0;
    });

    lua_pushlightuserdata(L, this);
    lua_pushlightuserdata(L, loop);
    lua_pushlightuserdata(L, initPromise);

    int ret = lua_pcall(L, 3, 0, 0);
    if (ret != LUA_OK) {
        initPromise->set_value(false);
        return;
    }

    lua_close(L);
}

void Kernel::luaRun(lua_State *L, uv_loop_t *loop, std::promise<bool> *initPromise)
{
    VOLCANO_ASSERT(L != nullptr);
    VOLCANO_ASSERT(loop != nullptr);
    VOLCANO_ASSERT(initPromise != nullptr);

    auto main = taskFromLua(L);
    main->data = this;
    main->loop = loop;

    luaL_openlibs(L);

    Registration reg(L);
    auto ns = reg.table("Volcano");
    initExports(ns);

    if (!loadInitrc(L)) {
        initPromise->set_value(false);
        return;
    }

    uv_prepare_t schedulePreparer;
    uv_prepare_init(loop, &schedulePreparer);
    uv_prepare_start(&schedulePreparer, [](uv_prepare_t *prepare) {
        auto L = reinterpret_cast<lua_State *>(prepare->data);
        auto kernel = fromLua(L);
        kernel->schedule(L);
    });
    schedulePreparer.data = L;

    Core::run(loop, initPromise);

    uv_close_sync(&schedulePreparer);
}

bool Kernel::loadInitrc(lua_State *L)
{
    return true;
}

void Kernel::schedule(lua_State *L)
{
    VolcanoListNode *node;
    Task *task;
    lua_State *T;
    int ret;
    int nresults;

    for (;;) {
        m_taskListMutex.lock();
        node = volcanoListRemoveHead(&m_taskListReady);
        m_taskListMutex.unlock();

        if (node == volcanoListKnot(&m_taskListReady))
            break;

        task = VOLCANO_MEMBEROF(node, Task, node);
        T = taskToLua(task);

        ret = lua_resume(T, L, 0, &nresults);
        if (ret != LUA_YIELD) {
            m_taskListMutex.lock();
            volcanoListMove(&m_taskListReady, &task->waitList);
            m_taskListMutex.unlock();
        }
    }

    m_taskListMutex.lock();
    bool trapListEmpty = volcanoListIsEmpty(&m_taskListTrapped);
    m_taskListMutex.unlock();
    if (!trapListEmpty)
        uv_async_send(&m_trapAsync);
}

int Kernel::doTrap(lua_State *L, lua_CFunction func)
{
    VOLCANO_ASSERT(func != nullptr);

    Task *task = taskFromLua(L);
    VOLCANO_ASSERT(task->trapFunc == nullptr);

    Kernel *kernel = fromTask(task);
    VOLCANO_ASSERT(std::this_thread::get_id() == kernel->thread().get_id());

    task->trapFunc = func;
    task->trapResult = TRAP_ERROR_NOT_SET;

    kernel->m_taskListMutex.lock();
    volcanoListAppend(&kernel->m_taskListTrapped, &task->node);
    kernel->m_taskListMutex.unlock();

    return lua_yieldk(L, 0, 0, [](lua_State *L, int status, lua_KContext ctx) -> int {
        VOLCANO_UNUSED(status);
        VOLCANO_UNUSED(ctx);

        Task *task = taskFromLua(L);
        VOLCANO_ASSERT(task->trapFunc != nullptr);

        int ret = task->trapResult;
        if (ret < 0)
          luaL_error(L, "trap %p error %d", task->trapFunc, ret);

        task->trapFunc = nullptr;
        task->trapResult = 0;

        return ret;
    });
}

///////////////////////////////////////////////////////////////////////////////
// VM exports (in VM thread)
///////////////////////////////////////////////////////////////////////////////

void Kernel::initExports(Registration &reg)
{
    reg.def("Task", &Kernel::sysTaskNew);
    reg.def("current", &Kernel::sysCurrent);
    reg.def("wait", &Kernel::sysWait);
    reg.def("sleep", &Kernel::sysSleep);
}

int Kernel::sysTaskNew(lua_State *L)
{
    lua_State *T = lua_newthread(L);
    int top = lua_gettop(L);
    if (top != 1)
        luaL_error(L, "one parameter.");

    int type = lua_type(L, 1);
    switch (type) {
    case LUA_TSTRING:
        luaL_loadstring(T, lua_tostring(L, 1));
        break;
    case LUA_TFUNCTION:
        lua_xmove(L, T, 1);
        break;
    default:
        luaL_error(L, "invalid paramter type.");
        break;
    }

    return 1;
}

int Kernel::sysCurrent(lua_State *L)
{
    lua_pushthread(L);
    return 1;
}

int Kernel::sysSleep(lua_State *L)
{
    int ms = (int)luaL_checkinteger(L, 1);
    Task *task = taskFromLua(L);
    Kernel *kernel = fromTask(task);

    kernel->m_taskListMutex.lock();
    volcanoListNodeUnlink(&task->node);

    if (ms < 1) {
        volcanoListAppend(&kernel->m_taskListReady, &task->node);
        kernel->m_taskListMutex.unlock();
    } else {
        kernel->m_taskListMutex.unlock();
        uv_timer_start(&task->sleepTimer, [](uv_timer_t *timer) {
            auto task = reinterpret_cast<Task *>(timer->data);
            auto kernel = fromTask(task);
            kernel->m_taskListMutex.lock();
            volcanoListNodeUnlink(&task->node);
            volcanoListAppend(&kernel->m_taskListReady, &task->node);
            kernel->m_taskListMutex.unlock();
        }, ms, 0);
    }

    return lua_yield(L, 0);
}

int Kernel::sysWait(lua_State *L)
{
    lua_State *T = nullptr;
    unsigned int timeout = 0;

    switch (lua_gettop(L)) {
    case 2:
        timeout = luaL_checkinteger(L, 2);
        luaL_argcheck(L, timeout >= 0, 2, "invalid timeout.");
    case 1:
        luaL_checktype(L, 1, LUA_TTHREAD);
        T = lua_tothread(L, 1);
        break;
    default:
        luaL_error(L, "too many parameters.");
        break;
    }

    Task *curr = taskFromLua(L);
    Task *task = taskFromLua(T);

    if (timeout > 0) {
        uv_timer_start(&curr->sleepTimer, [](uv_timer_t *timer) {
            auto task = reinterpret_cast<Task *>(timer->data);
            auto kernel = fromTask(task);
            task->sysResult = SYS_TIMEOUT;
            kernel->m_taskListMutex.lock();
            volcanoListNodeUnlink(&task->node);
            volcanoListAppend(&kernel->m_taskListReady, &task->node);
            kernel->m_taskListMutex.unlock();
        }, timeout, 0);
    }

    task->sysResult = SYS_OK;

    auto kernel = fromTask(task);
    kernel->m_taskListMutex.lock();
    volcanoListNodeUnlink(&task->node);
    volcanoListAppend(&task->waitList, &curr->node);
    kernel->m_taskListMutex.unlock();

    return lua_yieldk(L, 0, 0, [](lua_State *L, int status, lua_KContext ctx) {
        VOLCANO_UNUSED(status);
        VOLCANO_UNUSED(ctx);

        auto task = taskFromLua(L);
        uv_timer_stop(&task->sleepTimer);
        int ret = task->sysResult;
        task->sysResult = SYS_OK;

        return ret;
    });
}

VOLCANO_VM_END

///////////////////////////////////////////////////////////////////////////////
// Lua VM Hooks (called in VM thread)
///////////////////////////////////////////////////////////////////////////////

extern "C" void volcanoVMTaskAdded(lua_State *L, lua_State *L1)
{
    Volcano::VM::Kernel::taskAdded(L, L1);
}

extern "C" void volcanoVMTaskRemoved(lua_State *L, lua_State *L1)
{
    Volcano::VM::Kernel::taskRemoved(L, L1);
}

extern "C" void volcanoVMTaskResume(lua_State *L, int n)
{
    Volcano::VM::Kernel::taskResume(L, n);
}

extern "C" void volcanoVMTaskYield(lua_State *L, int n)
{
    Volcano::VM::Kernel::taskYield(L, n);
}
