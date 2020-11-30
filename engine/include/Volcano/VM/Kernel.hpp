//
//
#ifndef VOLCANO_VM_KERNEL_HPP
#define VOLCANO_VM_KERNEL_HPP

#include <string>
#include <thread>
#include <future>

#include <Volcano/List.h>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Registration.hpp>
#include <Volcano/VM/Core.hpp>

VOLCANO_VM_BEGIN

using Task = VolcanoVMTask;

class Kernel: public Core {
public:
    Kernel(uv_loop_t *loop);
    ~Kernel(void) override;

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

    void run(uv_loop_t *loop, std::promise<bool> *initPromise) override;
    virtual void initExports(Registration &reg);

private:
    void luaRun(lua_State *L, uv_loop_t *loop, std::promise<bool> *initPromise);
    bool loadInitrc(lua_State *L);
    void schedule(lua_State *L);
    void handleTraps(void);

private:
    static int sysTaskNew(lua_State *L);
    static int sysCurrent(lua_State *L);
    static int sysSleep(lua_State *L);
    static int sysWait(lua_State *L);

private:
    uv_async_t m_trapAsync;
    std::mutex m_taskListMutex;
    VolcanoList m_taskListReady;
    VolcanoList m_taskListTrapped;
};

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
