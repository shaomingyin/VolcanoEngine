/*
 *
 */
#ifndef VOLCANO_VM_TASK_H
#define VOLCANO_VM_TASK_H

#include <uv.h>

#include <Volcano/List.h>

VOLCANO_C_BEGIN

typedef struct {
	uv_timer_t sleep_timer;
	VolcanoListNode node;
	VolcanoList wait_list;
	void* data;
} VolcanoVmTask;

void volcanoVmInitHook(lua_State* L);
void volcanoVmShutdownHook(lua_State* L);
void volcanoVmAddTaskHook(lua_State* L, lua_State* T);
void volcanoVmRemoveTaskHook(lua_State* L, lua_State* T);
void volcanoVmResumeTaskHook(lua_State* T, int n);
void volcanoVmYieldTaskHook(lua_State* T, int n);

VOLCANO_C_END

#undef LUA_EXTRASPACE
#define LUA_EXTRASPACE sizeof(VolcanoVmTask)

#undef luai_userstateopen
#define luai_userstateopen(L) volcanoVmInitHook(L)

#undef luai_userstateclose
#define luai_userstateclose(L) volcanoVmShutdownHook(L)

#undef luai_userstatethread
#define luai_userstatethread(L, T) volcanoVmAddTaskHook(L, T)

#undef luai_userstatefree
#define luai_userstatefree(L, T) volcanoVmRemoveTaskHook(L, T)

#undef luai_userstateresume
#define luai_userstateresume(T, n) volcanoVmResumeTaskHook(T, n)

#undef luai_userstateyield
#define luai_userstateyield(T, n) volcanoVmYieldTaskHook(T, n)

#endif /* VOLCANO_VM_TASK_H */
