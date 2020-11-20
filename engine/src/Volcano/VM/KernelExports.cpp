//
//
#include <string>

#include <Volcano/Graphics/DirectionalLight.hpp>
#include <Volcano/Graphics/PointLight.hpp>
#include <Volcano/Graphics/SpotLight.hpp>
#include <Volcano/Graphics/Mesh.hpp>

#include <Volcano/VM/Kernel.hpp>

VOLCANO_VM_BEGIN

static const char *const DIRECTION_LIGHT_TYPE = "Volcano.DirectonLight";
static const char *const POINT_LIGHT_TYPE = "Volcano.PointLight";
static const char *const SPOT_LIGHT_TYPE = "Volcano.SpotLight";
static const char *const STATIC_MESH_TYPE = "Volcano.StaticMesh";
static const char *const DYNAMIC_MESH_TYPE = "Volcano.DynamicMesh";

void Kernel::initExports(lua_State *L)
{
    luaL_newmetatable(L, DIRECTION_LIGHT_TYPE);

    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, &Kernel::sysLightDestroy);
    lua_rawset(L, -2);

    lua_pop(L, 1);

    luaL_newmetatable(L, POINT_LIGHT_TYPE);
    lua_pop(L, 1);

    luaL_newmetatable(L, SPOT_LIGHT_TYPE);
    lua_pop(L, 1);

    luaL_newmetatable(L, STATIC_MESH_TYPE);
    lua_pop(L, 1);

    lua_newtable(L);

    lua_pushcfunction(L, &Kernel::sysTaskNew);
    lua_setfield(L, -2, "task_new");

    lua_pushcfunction(L, &Kernel::sysCurrent);
    lua_setfield(L, -2, "current");

    lua_pushcfunction(L, &Kernel::sysWait);
    lua_setfield(L, -2, "wait");

    lua_pushcfunction(L, &Kernel::sysSleep);
    lua_setfield(L, -2, "sleep");

    lua_pushcfunction(L, &Kernel::sysDirectionalLightNew);
    lua_setfield(L, -2, "directional_light_new");

    lua_pushcfunction(L, &Kernel::sysPointLightNew);
    lua_setfield(L, -2, "point_light_new");

    lua_pushcfunction(L, &Kernel::sysSpotLightNew);
    lua_setfield(L, -2, "spot_light_new");

    lua_pushcfunction(L, &Kernel::sysStaticMeshNew);
    lua_setfield(L, -2, "static_mesh_new");

    lua_pushcfunction(L, &Kernel::sysDynamicMeshNew);
    lua_setfield(L, -2, "dynamic_mesh_new");

    lua_setglobal(L, "Volcano");
}

int Kernel::sysCurrent(lua_State *L)
{
    lua_pushthread(L);
    return 1;
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

int Kernel::sysSleep(lua_State *L)
{
    int ms = (int)luaL_checkinteger(L, 1);
    Task *task = taskFromLua(L);
    Kernel *k = fromTask(task);

    std::unique_lock locker(k->m_taskListLock);
    volcanoListNodeUnlink(&task->node);

    if (ms < 1)
        volcanoListAppend(&k->m_taskListReady, &task->node);
    else
        uv_timer_start(&task->sleepTimer, sysSleepDone, ms, 0);

    return lua_yield(L, 0);
}

void Kernel::sysSleepDone(uv_timer_t *p)
{
    Task *task = (Task *)(p->data);
    Kernel *k = fromTask(task);

    std::unique_lock locker(k->m_taskListLock);
    volcanoListNodeUnlink(&task->node);
    volcanoListAppend(&k->m_taskListReady, &task->node);
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

    if (timeout > 0)
        uv_timer_start(&curr->sleepTimer, &Kernel::sysWaitTimeout, timeout, 0);

    task->sysResult = SYS_OK;

    std::unique_lock locker(fromTask(task)->m_taskListLock);
    volcanoListNodeUnlink(&task->node);
    volcanoListAppend(&task->waitList, &curr->node);

    return lua_yieldk(L, 0, 0, &Kernel::sysWaitDone);
}

void Kernel::sysWaitTimeout(uv_timer_t *p)
{
    Task *task = reinterpret_cast<Task *>(p->data);
    Kernel *k = fromTask(task);

    task->sysResult = SYS_TIMEOUT;

    std::unique_lock locker(k->m_taskListLock);
    volcanoListNodeUnlink(&task->node);
    volcanoListAppend(&k->m_taskListReady, &task->node);
}

int Kernel::sysWaitDone(lua_State *L, int status, lua_KContext ctx)
{
    VOLCANO_UNUSED(status);
    VOLCANO_UNUSED(ctx);

    Task *task = taskFromLua(L);
    int ret = task->sysResult;
    task->sysResult = SYS_OK;

    return ret;
}

int Kernel::sysDirectionalLightNew(lua_State *L)
{
    return doTrap(L, [](lua_State *L) {
        return 0;
    });
}

int Kernel::sysPointLightNew(lua_State *L)
{
    return doTrap(L, [](lua_State *L) {
        return 0;
    });
}

int Kernel::sysSpotLightNew(lua_State *L)
{
    return doTrap(L, [](lua_State *L) {
        return 0;
    });
}

int Kernel::sysLightDestroy(lua_State *L)
{
    return doTrap(L, [](lua_State *L) {
        return 0;
    });
}

int Kernel::sysStaticMeshNew(lua_State *L)
{
    return doTrap(L, [](lua_State *L) {
        return 0;
    });
}

int Kernel::sysDynamicMeshNew(lua_State *L)
{
    return doTrap(L, [](lua_State *L) {
        return 0;
    });
}

int Kernel::sysMeshDestroy(lua_State *L)
{
    return doTrap(L, [](lua_State *L) {
        return 0;
    });
}

VOLCANO_VM_END
