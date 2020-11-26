/*
 *
 */
#ifndef VOLCANO_VM_TASK_H
#define VOLCANO_VM_TASK_H

#include <uv.h>

#include <Volcano/List.h>

#ifdef LUA_EXTRASPACE
#    undef LUA_EXTRASPACE
#endif

#ifdef luai_userstatethread
#undef    luai_userstatethread
#endif

#ifdef luai_userstatefree
#undef    luai_userstatefree
#endif

#ifdef luai_userstateresume
#undef    luai_userstateresume
#endif

#ifdef luai_userstateyield
#undef    luai_userstateyield
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    VolcanoListNode node;
    uv_loop_t *loop;
    uv_timer_t sleepTimer;
    int sysResult;
    VolcanoList waitList;
    lua_CFunction trapFunc;
    int trapResult;
    void *data;
} VolcanoVMTask;

void volcanoVMTaskAdded(lua_State *L, lua_State *L1);
void volcanoVMTaskRemoved(lua_State *L, lua_State *L1);
void volcanoVMTaskResume(lua_State *L, int n);
void volcanoVMTaskYield(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#define LUA_EXTRASPACE sizeof(VolcanoVMTask)

#define luai_userstatethread(L, L1) volcanoVMTaskAdded(L, L1)
#define luai_userstatefree(L, L1) volcanoVMTaskRemoved(L, L1)
#define luai_userstateresume(L, n) volcanoVMTaskResume(L, n)
#define luai_userstateyield(L, n) volcanoVMTaskYield(L, n)

#define LUA_PROGNAME "volcano"

#endif /* VOLCANO_VM_TASK_H */
