//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Kernel.hpp>

VOLCANO_VM_BEGIN

void Kernel::initExports(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	lua_newtable(L);

	lua_pushcfunction(L, &Kernel::sysCurrent);
	lua_setfield(L, -2, "current");
	
	lua_pushcfunction(L, &Kernel::sysTask);
	lua_setfield(L, -2, "task");

	lua_pushcfunction(L, &Kernel::sysSleep);
	lua_setfield(L, -2, "sleep");

	lua_setglobal(L, "volcano");
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
		} else
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
		volcanoListAppend(&kernel->m_taskListReady, &task->node);

	return lua_yield(L, 0);
}

void Kernel::sysSleepDone(uv_timer_t *p)
{
	VOLCANO_LOGD(__func__);
	auto task = reinterpret_cast<Task *>(p->data);
	auto kernel = fromTask(task);

	volcanoListNodeUnlink(&task->node);
	volcanoListAppend(&kernel->m_taskListReady, &task->node);
}

VOLCANO_VM_END
