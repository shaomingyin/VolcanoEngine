//
//
#ifndef VOLCANO_VM_COMMON_HPP
#define VOLCANO_VM_COMMON_HPP

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <Volcano/Common.hpp>

#define VOLCANO_VM_BEGIN VOLCANO_BEGIN namespace VM {
#define VOLCANO_VM_END } VOLCANO_END

VOLCANO_VM_BEGIN

using Task = VolcanoVMTask;

VOLCANO_INLINE Task *taskFromLua(lua_State *L)
{
	return reinterpret_cast<Task *>(lua_getextraspace(L));
}

VOLCANO_INLINE lua_State *taskToLua(Task *task)
{
	return reinterpret_cast<lua_State *>(CX_PMOVB(task, LUA_EXTRASPACE));
}

VOLCANO_VM_END

#endif // VOLCANO_VM_COMMON_HPP
