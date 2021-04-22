//
//
#ifndef VOLCANO_VM_LUA_HPP
#define VOLCANO_VM_LUA_HPP

#include <stack>
#include <string>
#include <string_view>

#include <lua.hpp>

#include <Volcano/VM/Common.hpp>

#define VOLCANO_VM_LUA_BEGIN VOLCANO_VM_BEGIN namespace Lua {
#define VOLCANO_VM_LUA_END } VOLCANO_VM_END

VOLCANO_VM_LUA_BEGIN

VOLCANO_INLINE void push(lua_State *L, bool v)
{
	lua_pushboolean(L, v);
}

VOLCANO_INLINE void push(lua_State *L, int v)
{
	lua_pushinteger(L, v);
}

VOLCANO_INLINE void push(lua_State *L, long v)
{
	lua_pushinteger(L, v);
}

VOLCANO_INLINE void push(lua_State *L, double v)
{
	lua_pushnumber(L, v);
}

VOLCANO_INLINE void push(lua_State *L, std::string_view v)
{
	lua_pushstring(L, v.data());
}

VOLCANO_INLINE void push(lua_State *L, lua_CFunction fn)
{
	lua_pushcfunction(L, fn);
}

VOLCANO_VM_LUA_END

#endif // VOLCANO_VM_LUA_HPP
