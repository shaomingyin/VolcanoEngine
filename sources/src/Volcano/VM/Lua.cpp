//
//
#include <Volcano/VM/Lua.hpp>

VOLCANO_VM_LUA_BEGIN

static void initMetaTable(lua_State *L, const MetaTable *metaTable)
{
	if (metaTable->name != nullptr) {
		lua_pushstring(L, "__name");
		lua_pushcfunction(L, metaTable->name);
		lua_rawset(L, -3);
	}

	if (metaTable->gc != nullptr) {
		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, metaTable->gc);
		lua_rawset(L, -3);
	}

	if (metaTable->toString != nullptr) {
		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, metaTable->toString);
		lua_rawset(L, -3);
	}

	if (metaTable->index != nullptr) {
		lua_pushstring(L, "__index");
		lua_pushcfunction(L, metaTable->index);
		lua_rawset(L, -3);
	}

	if (metaTable->newIndex != nullptr) {
		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, metaTable->newIndex);
		lua_rawset(L, -3);
	}
}

void newTable(lua_State *L, const char *metaName, const MetaTable *metaTable)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(metaName != nullptr);
	VOLCANO_ASSERT(metaTable != nullptr);

	lua_newtable(L);

	if (luaL_newmetatable(L, metaName))
		initMetaTable(L, metaTable);

	lua_setmetatable(L, -2);
}

void setField(lua_State *L, const char *name, bool v)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(name != nullptr);

	lua_pushboolean(L, v);
	lua_setfield(L, -2, name);
}

void setField(lua_State *L, const char *name, int v)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(name != nullptr);

	lua_pushinteger(L, v);
	lua_setfield(L, -2, name);
}

void setField(lua_State *L, const char *name, lua_CFunction fn)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(name != nullptr);
	VOLCANO_ASSERT(fn != nullptr);

	lua_pushcfunction(L, fn);
	lua_setfield(L, -2, name);
}

void rawSetField(lua_State *L, const char *name, bool v)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(name != nullptr);

	lua_pushstring(L, name);
	lua_pushboolean(L, v);
	lua_rawset(L, -3);
}

void rawSetField(lua_State *L, const char *name, int v)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(name != nullptr);

	lua_pushstring(L, name);
	lua_pushinteger(L, v);
	lua_rawset(L, -3);
}

void rawSetField(lua_State *L, const char *name, lua_CFunction fn)
{
	VOLCANO_ASSERT(L != nullptr);
	VOLCANO_ASSERT(name != nullptr);
	VOLCANO_ASSERT(fn != nullptr);

	lua_pushstring(L, name);
	lua_pushcfunction(L, fn);
	lua_rawset(L, -3);
}

VOLCANO_VM_LUA_END
