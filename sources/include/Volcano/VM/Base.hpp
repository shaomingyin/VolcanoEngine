//
//
#ifndef VOLCANO_VM_BASE_HPP
#define VOLCANO_VM_BASE_HPP

#include <string>
#include <string_view>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <functional>

#include <physfs.h>

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Window.hpp>
#include <Volcano/VM/Sound.hpp>

VOLCANO_VM_BEGIN

class Base {
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
	Base(uv_loop_t *loop);
	virtual ~Base(void);

public:
	bool start(std::string_view rootPath, Traps *traps);
	void stop(void);
	void postEvent(const SDL_Event &event);
	uv_loop_t *loop(void);
	const std::string &rootPath(void) const;

protected:
	void syncTrap(void);
	virtual void run(uv_loop_t *loop, std::promise<bool> *initPromise) = 0;
	virtual void frame(float elapsed);
	virtual void handleEvent(const SDL_Event &event);
	virtual void handleTrap(Traps *traps);

private:
	void threadMain(std::promise<bool> *initPromise);
	static void frameHandler(uv_timer_t *p);
	static void trapHandler(uv_async_t *p);

private:
	static const int EVENT_QUEUE_ORDER = 6;
	static const int EVENT_QUEUE_SIZE = 1 << EVENT_QUEUE_ORDER;
	static const int EVENT_QUEUE_MASK = EVENT_QUEUE_SIZE - 1;

private:
	uv_loop_t *m_loop;
	uv_async_t m_trapAsync;
	std::string m_rootPath;
	Traps *m_traps;
	SDL_Event m_eventQueue[EVENT_QUEUE_SIZE];
	int64_t m_eventFirst;
	int64_t m_eventLast;
	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	uv_async_t m_quitAsync;
	uint64_t m_timeLast;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_BASE_HPP
