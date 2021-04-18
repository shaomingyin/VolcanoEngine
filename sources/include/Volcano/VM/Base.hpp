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

#include <Volcano/VM/Common.hpp>

VOLCANO_VM_BEGIN

class Base {
public:
	Base(uv_loop_t *loop, std::string_view rootPath);
	virtual ~Base(void);

public:
	bool start(void);
	void stop(void);
	void postEvent(const SDL_Event &event);
	uv_loop_t *loop(void);
	const std::string &rootPath(void) const;

protected:
	void syncTrap(void);
	virtual void run(uv_loop_t *loop, std::promise<bool> *initPromise) = 0;
	virtual void frame(float elapsed);
	virtual void handleEvent(const SDL_Event &event);
	virtual void handleTrap(void) = 0;

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
