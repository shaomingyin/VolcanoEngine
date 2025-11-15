//
//
#ifndef VOLCANO_VM_TASK_HPP
#define VOLCANO_VM_TASK_HPP

#include <Volcano/Vm/Common.hpp>

VOLCANO_VM_BEGIN

using Task = VolcanoVmTask;

inline Task* taskFromState(lua_State* L) {
	return reinterpret_cast<Task*>(lua_getextraspace(L));
}

inline lua_State* taskToState(Task* T) {
	return reinterpret_cast<lua_State*>(VOLCANO_PMOVB(T, LUA_EXTRASPACE));
}

VOLCANO_VM_END

#endif // VOLCANO_VM_TASK_HPP
