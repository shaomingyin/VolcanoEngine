//
//
#ifndef VOLCANO_VM_KERNEL_HPP
#define VOLCANO_VM_KERNEL_HPP

#include <string>
#include <string_view>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Lua.hpp>
#include <Volcano/VM/Renderer.hpp>
#include <Volcano/VM/Sound.hpp>
#include <Volcano/VM/Window.hpp>
#include <Volcano/VM/Base.hpp>

VOLCANO_VM_BEGIN

class Kernel: public Base {
public:
	using Task = VolcanoVMTask;

public:
	Kernel(uv_loop_t *loop);
	virtual ~Kernel(void);

public: // For lua
	static Task *taskFromLua(lua_State *L);
	static lua_State *taskToLua(Task *task);
	static Kernel *fromTask(Task *task);
	static Kernel *fromLua(lua_State *L);

	void addTaskHook(lua_State *L, lua_State *T);
	void removeTaskHook(lua_State *L, lua_State *T);
	void resumeTaskHook(lua_State *T, int n);
	void yieldTaskHook(lua_State *T, int n);

protected:
	void run(uv_loop_t *loop, std::promise<bool> *initPromise) override;
	void frame(float elapsed) override;
	void handleEvent(const SDL_Event &event) override;
	void handleTrap(Traps *traps) override;

private:
	void Main(lua_State *L, uv_loop_t *loop, std::promise<bool> *initPromise);
	bool loadInitrc(lua_State *L);
	static void schedule(uv_prepare_t *p);
	static int trapRequest(lua_State *T);
	static int trapDone(lua_State *T, int status, lua_KContext ctx);

private:
	void initExports(lua_State *L);

private: // volcano
	static int sysIndex(lua_State *L);
	static int sysNewIndex(lua_State *L);
	static int sysCurrent(lua_State *L);
	static int sysTask(lua_State *L);

	static int sysSleep(lua_State *L);
	static void sysSleepDone(uv_timer_t *p);

private: // volcano.window
	static int sysWindowIndex(lua_State *L);
	static int sysWindowNewIndex(lua_State *L);

private: // volcano.renderer
	static int sysRendererIndex(lua_State *L);
	static int sysRendererNewIndex(lua_State *L);

private:
	cx_list_t m_taskListReady;
	cx_list_t m_taskListTrap;
};

VOLCANO_INLINE Kernel::Task *Kernel::taskFromLua(lua_State *L)
{
	return reinterpret_cast<Task *>(lua_getextraspace(L));
}

VOLCANO_INLINE lua_State *Kernel::taskToLua(Task *task)
{
	return reinterpret_cast<lua_State *>(CX_PMOVB(task, LUA_EXTRASPACE));
}

VOLCANO_INLINE Kernel *Kernel::fromTask(Task *task)
{
	return reinterpret_cast<Kernel *>(task->data);
}

VOLCANO_INLINE Kernel *Kernel::fromLua(lua_State *L)
{
	return fromTask(taskFromLua(L));
}

VOLCANO_VM_END

#endif // VOLCANO_VM_KERNEL_HPP
