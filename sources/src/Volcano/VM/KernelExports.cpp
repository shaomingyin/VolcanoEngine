//
//
#include <stack>
#include <string>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Kernel.hpp>

VOLCANO_VM_BEGIN

void Kernel::initExports(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	luaL_newmetatable(L, "Volcano");

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, &Kernel::sysIndex);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, &Kernel::sysNewIndex);
	lua_rawset(L, -3);

	lua_newtable(L);
	luaL_setmetatable(L, "Volcano");

	// common

	lua_pushcfunction(L, &Kernel::sysCurrent);
	lua_setfield(L, -2, "current");
	
	lua_pushcfunction(L, &Kernel::sysTask);
	lua_setfield(L, -2, "task");

	lua_pushcfunction(L, &Kernel::sysSleep);
	lua_setfield(L, -2, "sleep");

	// window

	luaL_newmetatable(L, "Volcnao.Window");

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, &Kernel::sysWindowIndex);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, &Kernel::sysWindowNewIndex);
	lua_rawset(L, -3);

	lua_newtable(L);
	luaL_setmetatable(L, "Volcano.Window");

	lua_setfield(L, -2, "window");

	// renderer

	luaL_newmetatable(L, "Volcano.Renderer");

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, &Kernel::sysRendererIndex);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, &Kernel::sysRendererNewIndex);
	lua_rawset(L, -3);

	lua_setglobal(L, "volcano");
}

int Kernel::sysIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "version") == 0) {
		// TODO
	} else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

	return 0;
}

int Kernel::sysNewIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "visible") == 0) {
	} else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

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
	} else if (strcmp(name, "resizable") == 0) {
		// TODO
	} else if (strcmp(name, "size") == 0) {
		// TODO
	} else if (strcmp(name, "fullscreen") == 0) {
		// TODO
	} else if (strcmp(name, "title") == 0) {
		// TODO
	} else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

	return 0;
}

int Kernel::sysWindowNewIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "visible") == 0) {
		bool v = !!luaL_checkinteger(L, 3);
		// TODO
	} else if (strcmp(name, "resizable") == 0) {
		bool v = !!luaL_checkinteger(L, 3);
		// TODO
	} else if (strcmp(name, "size") == 0) {
		// TODO
	} else if (strcmp(name, "fullscreen") == 0) {
		bool v = !!luaL_checkinteger(L, 3);
		// TODO
	} else if (strcmp(name, "title") == 0) {
		// TODO
	} else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

	return 0;
}

int Kernel::sysRendererIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "clear") == 0) {
		// TODO
	} else if (strcmp(name, "clearColor") == 0) {
		// TODO
	} else if (strcmp(name, "drawTriangles") == 0) {
		// TODO
	} else if (strcmp(name, "drawNormals") == 0) {
		// TODO
	} else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

	return 0;
}

int Kernel::sysRendererNewIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "clear") == 0) {
		bool v = !!luaL_checkinteger(L, 3);
		// TODO
	} else if (strcmp(name, "clearColor") == 0) {
		// TODO
	} else if (strcmp(name, "drawTriangles") == 0) {
		// TODO
	} else if (strcmp(name, "clearColor") == 0) {
		// TODO
	} else
		luaL_error(L, "unknown field '%s' for 'volcano.window'.", name);

	return 0;
}

VOLCANO_VM_END
