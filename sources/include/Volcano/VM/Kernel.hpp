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

VOLCANO_VM_BEGIN

using Task = VolcanoVMTask;

class Kernel {
public:
	class Traps {
	public:
		virtual void output(void) = 0;
	};

public:
	Kernel(uv_loop_t *loop);
	virtual ~Kernel(void);

public:
	bool start(Traps *traps);
	void stop(void);
	void handleEvent(const SDL_Event &event);

public: // Methods for lua
	void addTaskHook(lua_State *L, lua_State *T);
	void removeTaskHook(lua_State *L, lua_State *T);
	void resumeTaskHook(lua_State *T, int n);
	void yieldTaskHook(lua_State *T, int n);

private:
	void trapHandler(void);

private:
	void threadMain(std::promise<bool> *initResult);
	void initExports(lua_State *L);
	static int luaMain(lua_State *L);
	void Main(lua_State *L, std::promise<bool> *initResult);
	bool loadInitrc(lua_State *L);
	int trap(lua_State *T);
	void schedule(lua_State *L);

private:
	uv_loop_t *m_hostLoop;
	Traps *m_traps;
	uv_async_t m_trapAsync;
	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;

private:
	bool m_threadRunning;
	uv_loop_t m_loop;
	uv_async_t m_quitAsync;
	VolcanoList m_taskListReady;
	VolcanoList m_taskListTrap;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_KERNEL_HPP
