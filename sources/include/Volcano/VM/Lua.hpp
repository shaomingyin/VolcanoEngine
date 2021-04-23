//
//
#ifndef VOLCANO_VM_LUA_HPP
#define VOLCANO_VM_LUA_HPP

#include <stack>
#include <string>
#include <string_view>
#include <functional>

#include <lua.hpp>

#include <Volcano/VM/Common.hpp>

#define VOLCANO_VM_LUA_BEGIN VOLCANO_VM_BEGIN namespace Lua {
#define VOLCANO_VM_LUA_END } VOLCANO_VM_END

VOLCANO_VM_LUA_BEGIN

struct MetaTable {
	MetaTable(void) :
		name(nullptr),
		gc(nullptr),
		toString(nullptr),
		index(nullptr),
		newIndex(nullptr)
	{
	}

	int (*name)(lua_State *L);
	int (*gc)(lua_State *L);
	int (*toString)(lua_State *L);
	int (*index)(lua_State *L);
	int (*newIndex)(lua_State *L);
};

void newTable(lua_State *L, const char *metaName, const MetaTable *metaTable);

void setField(lua_State *L, const char *name, bool v);
void setField(lua_State *L, const char *name, int v);
void setField(lua_State *L, const char *name, lua_CFunction fn);

void rawSetField(lua_State *L, const char *name, bool v);
void rawSetField(lua_State *L, const char *name, int v);
void rawSetField(lua_State *L, const char *name, lua_CFunction fn);

VOLCANO_VM_LUA_END

#endif // VOLCANO_VM_LUA_HPP
