//
//
#ifndef VOLCANO_VM_KERNEL_HPP
#define VOLCANO_VM_KERNEL_HPP

#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Window.hpp>
#include <Volcano/VM/Sound.hpp>

VOLCANO_VM_BEGIN

class Kernel {
public:
	class Traps {
	public:
		Traps(void) = default;
		virtual ~Traps(void) = default;

	public:
		virtual Window *window(void) = 0;
		virtual Sound *sound(void) = 0;
	};

public:
	Kernel(uv_loop_t *loop);
	virtual ~Kernel(void);

public:
	bool start(Traps *traps);
	void stop(void);
	void postEvent(const SDL_Event &event);
	uv_loop_t *loop(void);

public: // For lua
	static Kernel *fromTask(Task *task);
	static Kernel *fromLua(lua_State *L);

	void addTaskHook(lua_State *L, lua_State *T);
	void removeTaskHook(lua_State *L, lua_State *T);
	void resumeTaskHook(lua_State *T, int n);
	void yieldTaskHook(lua_State *T, int n);

private:
	void threadMain(std::promise<bool> *initPromise);
	void protectedMain(lua_State *L, uv_loop_t *loop, std::promise<bool> *initPromise);
	bool loadInitrc(lua_State *L);
	void frame(float elapsed);
	static void schedule(uv_prepare_t *p);
	void handleEvent(const SDL_Event &event);
	void handleTraps(void);
	static int trapRequest(lua_State *T, lua_CFunction fn, lua_KFunction cb = nullptr, lua_KContext ctx = 0);
	static int trapDone(lua_State *T, int status, lua_KContext ctx);

private:
	void openLibs(lua_State *L);

	static int sysVersion(lua_State *L);

	static int sysCurrent(lua_State *L);

	static int sysTask(lua_State *L);

	static int sysSleep(lua_State *L);
	static void sysSleepDone(uv_timer_t *p);

	static int sysWindowIsVisible(lua_State *L);
	static int sysWindowSetVisible(lua_State *L);

	static int sysWindowSize(lua_State *L);
	static int sysWindowSetSize(lua_State *L);

	static int sysWindowIsFullscreen(lua_State *L);
	static int sysWindowSetFullscreen(lua_State *L);

	static int sysWindowTitle(lua_State *L);
	static int sysWindowSetTitle(lua_State *L);

	static int sysWindowRendererViewport(lua_State *L);
	static int sysWindowRendererSetViewport(lua_State *L);

	static int sysWindowRendererIsClearEnabled(lua_State *L);
	static int sysWindowRendererSetClearEnabled(lua_State *L);

	static int sysWindowRendererClearColor(lua_State *L);
	static int sysWindowRendererSetClearColor(lua_State *L);

	static int sysWindowRendererIsDrawTriangles(lua_State *L);
	static int sysWindowRendererSetDrawTriangles(lua_State *L);

	static int sysWindowRendererIsDrawNormals(lua_State *L);
	static int sysWindowRendererSetDrawNormals(lua_State *L);

	static int sysSoundVolume(lua_State *L);
	static int sysSoundSetVolume(lua_State *L);

private:
	static const int EVENT_QUEUE_ORDER = 6;
	static const int EVENT_QUEUE_SIZE = 1 << EVENT_QUEUE_ORDER;
	static const int EVENT_QUEUE_MASK = EVENT_QUEUE_SIZE - 1;

private:
	uv_loop_t *m_loop;
	uv_async_t m_trapAsync;
	Traps *m_traps;
	SDL_Event m_eventQueue[EVENT_QUEUE_SIZE];
	int64_t m_eventFirst;
	int64_t m_eventLast;
	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	uv_async_t m_quitAsync;
	uint64_t m_timeLast;
	cx_list_t m_taskListReady;
	cx_list_t m_taskListTrap;
};

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
