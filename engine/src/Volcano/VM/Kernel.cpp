//
//
#include <Volcano/VM/Kernel.hpp>

VOLCANO_VM_BEGIN

Kernel::Kernel(Object *parent):
    Object(parent),
    m_trapAsync([this](void) { handleTraps(); })
{
}

Kernel::~Kernel(void)
{
}

bool Kernel::start(const std::string &rootPath, const std::string &initPath)
{
    if (!m_fs.init(rootPath))
        return false;

    return true;
}

void Kernel::stop(void)
{
    VOLCANO_ASSERT(std::this_thread::get_id() != threadId());

    Object::postQuit(threadId());
}

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
    volcanoListAppend(&fromTask(task)->m_taskListReady, &task->node);
}

void Kernel::taskRemoved(lua_State *L, lua_State *L1)
{
    VOLCANO_UNUSED(L);

    Task *task = taskFromLua(L1);

    uv_timer_stop(&task->sleepTimer);
    uvSyncClose(&task->sleepTimer);
    volcanoListNodeUnlink(&task->node);
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

bool Kernel::onEvent(const SDL_Event &evt)
{
    return Object::onEvent(evt);
}

Duration Kernel::onUpdate(void)
{
    return Object::onUpdate();
}

void Kernel::run(uv_loop_t *loop)
{
    volcanoListReset(&m_taskListReady);
    volcanoListReset(&m_taskListTrapped);

    lua_State *L = luaL_newstate();
    if (L == nullptr) {
        setInitResult(false);
        return;
    }

    lua_pushcfunction(L, [](lua_State *L) -> int {
        auto k = reinterpret_cast<Kernel *>(lua_touserdata(L, 1));
        uv_loop_t *loop = reinterpret_cast<uv_loop_t *>(lua_touserdata(L, 2));
        k->kmain(L, loop);
        return 0;
    });

    lua_pushlightuserdata(L, this);
    lua_pushlightuserdata(L, loop);
    lua_pcall(L, 2, 0, 0);
    lua_close(L);
}

void Kernel::kmain(lua_State *L, uv_loop_t *loop)
{
    luaL_openlibs(L);

    initExports(L);

    auto main = taskFromLua(L);
    main->kernel = this;
    main->loop = loop;

    if (!loadInitrc(L)) {
        setInitResult(false);
        return;
    }

    uv_prepare_t schedulePreparer;
    uv_prepare_init(loop, &schedulePreparer);
    uv_prepare_start(&schedulePreparer, &Kernel::schedule);
    schedulePreparer.data = L;

    Core::run(loop);

    uv_prepare_stop(&schedulePreparer);
    uvSyncClose(&schedulePreparer);
}

bool Kernel::loadInitrc(lua_State *L)
{
    return true;
}

void Kernel::schedule(uv_prepare_t *p)
{
    VolcanoListNode *node;
    Task *task;
    lua_State *T;
    int ret;
    int nresults;
    lua_State *L = reinterpret_cast<lua_State *>(p->data);
    Kernel *k = fromLua(L);

    for (;;) {
        k->m_taskListLock.lock();
        node = volcanoListRemoveHead(&k->m_taskListReady);
        k->m_taskListLock.unlock();
        if (node == volcanoListKnot(&k->m_taskListReady))
            break;
        task = VOLCANO_MEMBEROF(node, Task, node);
        T = taskToLua(task);
        ret = lua_resume(T, L, 0, &nresults);
        if (ret != LUA_YIELD) {
            k->m_taskListLock.lock();
            volcanoListMove(&k->m_taskListReady, &task->waitList);
            k->m_taskListLock.unlock();
        }
    }

    k->m_taskListLock.lock();
    if (!volcanoListIsEmpty(&k->m_taskListTrapped)) {
        k->m_taskListLock.unlock();
        uv_async_send(&k->m_trapAsync);
    } else
        k->m_taskListLock.unlock();
}

void Kernel::handleTraps(uv_async_t *p)
{
    auto k = reinterpret_cast<Kernel *>(p->data);
    VolcanoListNode *node;
    Task *task;
    bool handled = false;

    for (;;) {
        k->m_taskListLock.lock();
        node = volcanoListRemoveHead(&k->m_taskListTrapped);
        k->m_taskListLock.unlock();

        if (node != volcanoListKnot(&k->m_taskListTrapped)) {
            task = VOLCANO_MEMBEROF(node, Task, node);
            task->trapResult = task->trapFunc(taskToLua(task));
            handled = true;
        } else
            break;

        k->m_taskListLock.lock();
        volcanoListAppend(&k->m_taskListReady, &task->node);
        k->m_taskListLock.unlock();
    }

    if (handled)
        k->kick();
}

int Kernel::trapDone(lua_State *L, int status, lua_KContext ctx)
{
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
}

int Kernel::doTrap(lua_State *L, lua_CFunction func)
{
    VOLCANO_ASSERT(func != nullptr);

    Task *task = taskFromLua(L);
    VOLCANO_ASSERT(task->trapFunc == nullptr);

    Kernel *k = fromTask(task);
    VOLCANO_ASSERT(std::this_thread::get_id() == k->thread().get_id());

    task->trapFunc = func;
    task->trapResult = TRAP_ERROR_NOT_SET;

    k->m_taskListLock.lock();
    volcanoListAppend(&k->m_taskListTrapped, &task->node);
    k->m_taskListLock.unlock();

    return lua_yieldk(L, 0, 0, &Kernel::trapDone);
}

VOLCANO_VM_END

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
