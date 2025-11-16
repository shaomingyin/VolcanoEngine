//
//
#include <Volcano/Vm/Kernel.hpp>

VOLCANO_VM_BEGIN

int Kernel::sysSleep(lua_State* L) noexcept {
	auto task = taskFromState(L);
	VOLCANO_ASSERT(task->data == this);
	unsigned int ms = static_cast<unsigned int>(luaL_checkinteger(L, 1));
	uv_timer_start(&task->sleep_timer, [](uv_timer_t* handle) {
		auto t = VOLCANO_MEMBEROF(handle, Task, sleep_timer);
		auto k = reinterpret_cast<Kernel*>(t->data);
		volcanoListNodeUnlink(&t->node);
		volcanoListAppend(&k->ready_task_list_, &t->node);
	}, ms, 0);
	return lua_yield(L, 0);
}

int Kernel::sysStart(lua_State* L) noexcept {
	luaL_checktype(L, 1, LUA_TFUNCTION);
	auto task = taskFromState(L);
	VOLCANO_ASSERT(task->data == this);
	auto new_thread = lua_newthread(L);
	if (new_thread == nullptr) {
		return luaL_error(L, "Failed to create new lua thread for task");
	}
	lua_pushvalue(L, 1);
	lua_xmove(L, new_thread, 1);
	return 1;
}

VOLCANO_VM_END
