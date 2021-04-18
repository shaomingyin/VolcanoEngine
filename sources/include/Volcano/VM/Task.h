/*
 *
 */
#ifndef VOLCANO_VM_TASK_H
#define VOLCANO_VM_TASK_H

#include <Volcano/Common.h>
#include <Volcano/List.h>

#ifdef LUA_EXTRASPACE
#	undef LUA_EXTRASPACE
#endif

#ifdef luai_userstatethread
#   undef luai_userstatethread
#endif

#ifdef luai_userstatefree
#   undef luai_userstatefree
#endif

#ifdef luai_userstateresume
#   undef luai_userstateresume
#endif

#ifdef luai_userstateyield
#   undef luai_userstateyield
#endif

VOLCANO_C_BEGIN

typedef struct {
	VolcanoListNode node;
	uv_loop_t *loop;
	uv_timer_t sleepTimer;
	int trapResult;
	void *kernel;
} VolcanoVMTask;

void volcanoVMAddTaskHook(lua_State *L, lua_State *T);
void volcanoVMRemoveTaskHook(lua_State *L, lua_State *T);
void volcanoVMResumeTaskHook(lua_State *T, int n);
void volcanoVMYieldTaskHook(lua_State *T, int n);

VOLCANO_C_END

#define LUA_EXTRASPACE sizeof(VolcanoVMTask)

#define luai_userstatethread(L, T) volcanoVMAddTaskHook(L, T)
#define luai_userstatefree(L, T) volcanoVMRemoveTaskHook(L, T)
#define luai_userstateresume(T, n) volcanoVMResumeTaskHook(T, n)
#define luai_userstateyield(T, n) volcanoVMYieldTaskHook(T, n)

#endif /* VOLCANO_VM_TASK_H */
