//
//
#ifndef VOLCANO_VM_KERNEL_HPP
#define VOLCANO_VM_KERNEL_HPP

#include <Volcano/List.h>
#include <Volcano/Object.hpp>
#include <Volcano/Async.hpp>
#include <Volcano/Timer.hpp>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/RootFileSystem.hpp>

VOLCANO_VM_BEGIN

using Task = VolcanoVMTask;

class Kernel: public Object {
public:
    Kernel(Object *parent);
    virtual ~Kernel(void);

public:
    bool start(const std::string &rootPath, const std::string &initPath);
    void stop(void);

public: // for lua
    static void taskAdded(lua_State *L, lua_State *L1);
    static void taskRemoved(lua_State *L, lua_State *L1);
    static void taskResume(lua_State *L, int n);
    static void taskYield(lua_State *L, int n);

protected:
    bool onEvent(const SDL_Event &evt) override;
    Duration onUpdate(void) override;

private:
    enum {
        SYS_OK = 0,
        SYS_INVALID_PARAM,
        SYS_TIMEOUT
    };

    enum {
        TRAP_TYPE_INVALID = -1,
        TRAP_TYPE_DIRECTIONAL_LIGHT_NEW = 0,
        TRAP_TYPE_POINT_LIGHT_NEW,
        TRAP_TYPE_SPOT_LIGHT_NEW,
        TRAP_TYPE_LIGHT_DESTROY,
        TRAP_TYPE_STATIC_MESH_NEW,
        TRAP_TYPE_DYNAMIC_MESH_NEW,
        TRAP_TYPE_MESH_DESTROY,
        TRAP_TYPE_MAX
    };

    enum {
        TRAP_ERROR_NOT_SET = -1,
        TRAP_ERROR_UNKNOWN = -2
    };

    static Task *taskFromLua(lua_State *L);
    static lua_State *taskToLua(Task *task);
    static Kernel *fromTask(Task *task);
    static Kernel *fromLua(lua_State *L);

    void kmain(lua_State *L, uv_loop_t *loop);
    void initExports(lua_State *L);
    bool loadInitrc(lua_State *L);
    static void schedule(uv_prepare_t *p);
    void handleTraps(void);
    static int trapDone(lua_State *L, int status, lua_KContext ctx);
    static int doTrap(lua_State *L, lua_CFunction func);

private:
    static int sysCurrent(lua_State *L);
    static int sysTaskNew(lua_State *L);
    static int sysSleep(lua_State *L);
    static void sysSleepDone(uv_timer_t *p);
    static int sysWait(lua_State *L);
    static void sysWaitTimeout(uv_timer_t *p);
    static int sysWaitDone(lua_State *L, int status, lua_KContext ctx);
    static int sysDirectionalLightNew(lua_State *L);
    static int sysPointLightNew(lua_State *L);
    static int sysSpotLightNew(lua_State *L);
    static int sysLightDestroy(lua_State *L);
    static int sysStaticMeshNew(lua_State *L);
    static int sysDynamicMeshNew(lua_State *L);
    static int sysMeshDestroy(lua_State *L);

private:
    RootFileSystem m_fs;
    VolcanoList m_taskListReady;
    VolcanoList m_taskListTrapped;
    Async m_trapAsync;
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
