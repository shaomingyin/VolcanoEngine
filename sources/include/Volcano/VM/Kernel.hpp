//
//
#ifndef VOLCANO_VM_KERNEL_HPP
#define VOLCANO_VM_KERNEL_HPP

#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <functional>

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Base.hpp>

VOLCANO_VM_BEGIN

class Kernel: public Base {
public:
	using Task = VolcanoVMTask;

	class Traps {
	public:
		virtual void output(void) = 0;
	};

public:
	Kernel(uv_loop_t *loop, std::string_view rootPath);
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
	void handleTrap(void) override;

private:
	void initExports(lua_State *L);
	void Main(lua_State *L, uv_loop_t *loop, std::promise<bool> *initPromise);
	bool loadInitrc(lua_State *L);
	static void schedule(uv_prepare_t *p);
	static int trapRequest(lua_State *T);
	static int trapDone(lua_State *T, int status, lua_KContext ctx);

	// volcano.current
	static int sysCurrent(lua_State *L);

	// volcano.task
	static int sysTask(lua_State *L);

	// volcano.sleep
	static int sysSleep(lua_State *L);
	static void sysSleepDone(uv_timer_t *p);

private:
	VolcanoList m_taskListReady;
	VolcanoList m_taskListTrap;
};

VOLCANO_INLINE Kernel::Task *Kernel::taskFromLua(lua_State *L)
{
	return reinterpret_cast<Task *>(lua_getextraspace(L));
}

VOLCANO_INLINE lua_State *Kernel::taskToLua(Task *task)
{
	return reinterpret_cast<lua_State *>(VOLCANO_PMOVB(task, LUA_EXTRASPACE));
}

VOLCANO_INLINE Kernel *Kernel::fromTask(Task *task)
{
	return reinterpret_cast<Kernel *>(task->kernel);
}

VOLCANO_INLINE Kernel *Kernel::fromLua(lua_State *L)
{
	return fromTask(taskFromLua(L));
}

VOLCANO_VM_END

#endif // VOLCANO_VM_KERNEL_HPP
