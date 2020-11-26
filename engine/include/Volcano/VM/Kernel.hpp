//
//
#ifndef VOLCANO_VM_KERNEL_HPP
#define VOLCANO_VM_KERNEL_HPP

#include <string>
#include <thread>
#include <future>

#include <Volcano/List.h>
#include <Volcano/SpinLock.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/RootFileSystem.hpp>
#include <Volcano/VM/World.hpp>
#include <Volcano/VM/Registration.hpp>

VOLCANO_VM_BEGIN

using Task = VolcanoVMTask;

class Kernel: public Noncopyable {
public:
    Kernel(uv_loop_t *loop);
    virtual ~Kernel(void);

public:
    bool start(const std::string &rootPath, const std::string &initPath);
    void stop(void);
    virtual void postEvent(const SDL_Event &evt);
    uv_loop_t *loop(void);
    const std::thread &thread(void) const;
    World &world(void);

public: // for lua
    static void taskAdded(lua_State *L, lua_State *L1);
    static void taskRemoved(lua_State *L, lua_State *L1);
    static void taskResume(lua_State *L, int n);
    static void taskYield(lua_State *L, int n);

protected:
    enum {
        SYS_OK = 0,
        SYS_INVALID_PARAM,
        SYS_TIMEOUT
    };

    enum {
        TRAP_ERROR_NOT_SET = -1,
        TRAP_ERROR_UNKNOWN = -2
    };

    static Task *taskFromLua(lua_State *L);
    static lua_State *taskToLua(Task *task);
    static Kernel *fromTask(Task *task);
    static Kernel *fromLua(lua_State *L);

    static int doTrap(lua_State *L, lua_CFunction func);

    virtual bool init(void);
    virtual void shutdown(void);
    virtual void initExports(Registration &reg);
    virtual void frame(float elapsed);
    virtual void handleEvent(const SDL_Event &evt);

private:
    void threadMain(std::promise<bool> *initResult);
    void luaMain(lua_State *L, std::promise<bool> &initPromise);
    bool loadInitrc(lua_State *L);
    void schedule(lua_State *L);
    void handleTraps(void);

private:
    static int sysTaskNew(lua_State *L);
    static int sysCurrent(lua_State *L);
    static int sysSleep(lua_State *L);
    static int sysWait(lua_State *L);

private:
    static const int EVENT_QUEUE_ORDER = 6;
    static const int EVENT_QUEUE_SIZE = 1 << EVENT_QUEUE_ORDER;
    static const int EVENT_QUEUE_MASK = EVENT_QUEUE_SIZE - 1;

private:
    uv_loop_t *m_loop;
    bool m_started;
    std::thread m_thread;
    uv_async_t m_trapAsync;
    uv_async_t m_quitAsync;
    uv_async_t m_kickAsync;
    std::string m_rootPath;
    std::string m_initPath;
    SDL_Event m_eventQueue[EVENT_QUEUE_SIZE];
    uint64_t m_eventFirst;
    uint64_t m_eventLast;
    RootFileSystem m_fs;
    World m_world;
    std::mutex m_taskListMutex;
    VolcanoList m_taskListReady;
    VolcanoList m_taskListTrapped;
    int64_t m_lastFrameTime;
};

VOLCANO_INLINE uv_loop_t *Kernel::loop(void)
{
    return m_loop;
}

VOLCANO_INLINE const std::thread &Kernel::thread(void) const
{
    return m_thread;
}

VOLCANO_INLINE World &Kernel::world(void)
{
    VOLCANO_ASSERT(std::this_thread::get_id() == m_thread.get_id());

    return m_world;
}

VOLCANO_INLINE Task *Kernel::taskFromLua(lua_State *L)
{
    VOLCANO_ASSERT(L != nullptr);

    return (Task *)lua_getextraspace(L);
}

VOLCANO_INLINE lua_State *Kernel::taskToLua(Task *task)
{
    VOLCANO_ASSERT(task != nullptr);

    return (lua_State *)VOLCANO_PMOVB(task, LUA_EXTRASPACE);
}

VOLCANO_INLINE Kernel *Kernel::fromTask(Task *task)
{
    VOLCANO_ASSERT(task != nullptr);

    return reinterpret_cast<Kernel *>(task->data);
}

VOLCANO_INLINE Kernel *Kernel::fromLua(lua_State *L)
{
    VOLCANO_ASSERT(L != nullptr);

    return fromTask(taskFromLua(L));
}

VOLCANO_VM_END

#endif // VOLCANO_VM_KERNEL_HPP
