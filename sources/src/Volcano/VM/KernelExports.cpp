//
//
#include <stack>
#include <string>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Kernel.hpp>

VOLCANO_VM_BEGIN

class test {
public:
	test(void) { }
	~test(void) { }
};

void Kernel::newExports(lua_State *L)
{
	VOLCANO_ASSERT(L != nullptr);

	Lua::MetaTable sysMetaTable;
	Lua::MetaTable sysWindowMetaTable;
	Lua::MetaTable sysWindowRendererMetaTable;

	sysMetaTable.index = &Kernel::sysIndex;
	sysMetaTable.newIndex = &Kernel::sysNewIndex;

	sysWindowMetaTable.index = &Kernel::sysWindowIndex;
	sysWindowMetaTable.newIndex = &Kernel::sysWindowNewIndex;

	sysWindowRendererMetaTable.index = &Kernel::sysWindowRendererIndex;
	sysWindowRendererMetaTable.newIndex = &Kernel::sysWindowRendererNewIndex;

	Lua::newTable(L, "Volcano", &sysMetaTable);
		Lua::rawSetField(L, "current", &Kernel::sysCurrent);
		Lua::rawSetField(L, "task", &Kernel::sysTask);
		Lua::rawSetField(L, "sleep", &Kernel::sysSleep);
		lua_pushstring(L, "window");
		Lua::newTable(L, "Window", &sysWindowMetaTable);
			Lua::newTable(L, "renderer", "Renderer", &sysWindowRendererMetaTable);
			lua_setfield
		Lua::endTable(L);
}

int Kernel::sysIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "version") == 0) {
		// TODO
	} else
		lua_pushvalue(L, 1);

	return 0;
}

int Kernel::sysNewIndex(lua_State *L)
{
	const char *name = lua_tostring(L, 2);

	if (strcmp(name, "visible") == 0) {
	} else
		luaL_error(L, "unknown symbol 'volcano.%s'", name);

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

int Kernel::sysWindowRendererIndex(lua_State *L)
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

int Kernel::sysWindowRendererNewIndex(lua_State *L)
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
