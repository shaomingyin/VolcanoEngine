//
//
#include <chrono>
#include <filesystem>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Base.hpp>

VOLCANO_VM_BEGIN

using namespace std::chrono;

Base::Base(uv_loop_t *loop):
	m_loop(loop)
{
	VOLCANO_ASSERT(m_loop != nullptr);

	uv_async_init(m_loop, &m_trapAsync, &Base::trapHandler);
	m_trapAsync.data = this;
}

Base::~Base(void)
{
}

bool Base::start(std::string_view rootPath, Traps *traps)
{
	VOLCANO_ASSERT(m_loop != nullptr);
	VOLCANO_ASSERT(traps != nullptr);

	if (!std::filesystem::is_directory(m_rootPath)) {
		VOLCANO_LOGE("Root path '%s' is not a directory.", m_rootPath.c_str());
		return false;
	}

	m_rootPath = rootPath;
	m_traps = traps;

	std::promise<bool> initPromise;
	auto initResult = initPromise.get_future();
	m_thread = std::thread(&Base::threadMain, this, &initPromise);
	auto state = initResult.wait_for(15s);

	return (state == std::future_status::ready);
}

void Base::stop(void)
{
	if (m_thread.joinable()) {
		uv_async_send(&m_quitAsync);
		m_thread.join();
	}
}

void Base::postEvent(const SDL_Event &event)
{
	m_eventQueue[m_eventLast & EVENT_QUEUE_MASK] = event;
	m_eventLast += 1;
}

uv_loop_t *Base::loop(void)
{
	return m_loop;
}

const std::string &Base::rootPath(void) const
{
	return m_rootPath;
}

void Base::syncTrap(void)
{
	uv_async_send(&m_trapAsync);
	m_cond.wait(m_mutex);
}

void Base::run(uv_loop_t *loop, std::promise<bool> *initPromise)
{
	VOLCANO_ASSERT(loop != nullptr);
	VOLCANO_ASSERT(initPromise != nullptr);

	uv_timer_t frameTimer;
	uv_timer_init(loop, &frameTimer);
	uv_timer_start(&frameTimer, &Base::frameHandler, 0, 16);
	frameTimer.data = this;

	ScopeGuard updateTimerGuard([&] { uv_timer_stop(&frameTimer); });

	initPromise->set_value(true);

	uv_run(loop, UV_RUN_DEFAULT);
}

void Base::frame(float elapsed)
{
	while (m_eventFirst < m_eventLast) {
		handleEvent(m_eventQueue[m_eventFirst & EVENT_QUEUE_MASK]);
		m_eventFirst += 1;
	}
}

void Base::handleEvent(const SDL_Event &event)
{
}

void Base::handleTrap(Traps *traps)
{
	m_cond.notify_all();
}

void Base::threadMain(std::promise<bool> *initPromise)
{
	VOLCANO_ASSERT(initPromise != nullptr);

	std::lock_guard<std::mutex> locker(m_mutex);
	
	uv_loop_t loop;

	if (uv_loop_init(&loop) < 0) {
		VOLCANO_LOGE("Failed to init vm loop.");
		initPromise->set_value(false);
		return;
	}

	ScopeGuard uvGuard([&] { uv_loop_close(&loop); });

	if (!PHYSFS_init("volcano")) {
		VOLCANO_LOGE("Failed to init physfs.");
		initPromise->set_value(false);
		return;
	}

	ScopeGuard physfsGuard([] { PHYSFS_deinit(); });

	if (!PHYSFS_setWriteDir(m_rootPath.c_str())) {
		VOLCANO_LOGE("Failed to set write dir to '%s'.", m_rootPath.c_str());
		initPromise->set_value(false);
		return;
	}

	if (!PHYSFS_mount(m_rootPath.c_str(), "/", 0)) {
		VOLCANO_LOGE("Failed to mount root path '%s'.", m_rootPath.c_str());
		initPromise->set_value(false);
		return;
	}

	run(&loop, initPromise);
}

void Base::frameHandler(uv_timer_t *p)
{
	auto core = reinterpret_cast<Base *>(p->data);
	VOLCANO_ASSERT(core != nullptr);
	uint64_t curr = uv_now(p->loop);
	uint64_t pass = curr - core->m_timeLast;
	core->frame(float(pass));
	core->m_timeLast = curr;
	uv_update_time(p->loop);
}

void Base::trapHandler(uv_async_t *p)
{
	auto base = reinterpret_cast<Base *>(p->data);
	VOLCANO_ASSERT(base != nullptr);
	std::lock_guard<std::mutex> locker(base->m_mutex);
	base->handleTrap(base->m_traps);
}

VOLCANO_VM_END
