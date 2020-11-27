//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/KernelBase.hpp>

VOLCANO_VM_BEGIN

KernelBase::KernelBase(uv_loop_t *loop):
    m_loop(loop),
    m_started(false),
    m_eventFirst(0),
    m_eventLast(0)
{
    VOLCANO_ASSERT(m_loop != nullptr);
}

KernelBase::~KernelBase(void)
{
    if (m_started)
        stop();
    else if (m_thread.joinable())
        m_thread.join();
}

bool KernelBase::start(const std::string &rootPath, const std::string &initPath)
{
    m_eventFirst = 0;
    m_eventLast = 0;

    m_rootPath = rootPath;
    m_initPath = initPath;

    if (!init())
        return false;

    std::promise<bool> initPromise;
    auto initFuture = initPromise.get_future();

    m_thread = std::thread(&KernelBase::threadMain, this, &initPromise);

    m_started = initFuture.get();
    if (!m_started)
        return false;

    return true;
}

void KernelBase::stop(void)
{
    VOLCANO_ASSERT(m_started);

    uv_async_send(&m_quitAsync);
    m_thread.join();

    shutdown();

    m_started = false;
}

void KernelBase::postEvent(const SDL_Event &evt)
{
    if (evt.type != SDL_WINDOWEVENT) {
        m_eventQueue[m_eventLast & EVENT_QUEUE_MASK] = evt;
        m_eventLast += 1;
    }
}

bool KernelBase::init(void)
{
    return true;
}

void KernelBase::shutdown(void)
{
}

void KernelBase::threadMain(std::promise<bool> *initPromise)
{
    VOLCANO_SCOPE_TRACE(tmain);

    VOLCANO_ASSERT(initPromise != nullptr);

    uv_loop_t loop;
    uv_loop_init(&loop);

    uv_async_init(&loop, &m_quitAsync, [](uv_async_t *async) {
        uv_stop(async->loop);
    });

    uv_async_init(&loop, &m_kickAsync, [](uv_async_t *async) {
        VOLCANO_UNUSED(async);
    });

    uv_timer_t frameTimer;
    uv_timer_init(&loop, &frameTimer);
    uv_timer_start(&frameTimer, [](uv_timer_t *timer) {
        auto kernelBase = reinterpret_cast<KernelBase *>(timer->data);
        auto now = uv_now(timer->loop);
        auto elapsed = now - kernelBase->m_lastFrameTime;
        kernelBase->frame(float(elapsed) / 1000.0f);
        kernelBase->m_lastFrameTime = now;
    }, 0, 16);
    frameTimer.data = this;

    run(&loop);

    uv_close_sync(&frameTimer);
    uv_close_sync(&m_kickAsync);
    uv_close_sync(&m_quitAsync);

    uv_loop_close(&loop);
}

void KernelBase::run(uv_loop_t *loop)
{
    uv_update_time(loop);
    m_lastFrameTime = uv_now(loop);

    //initPromise.set_value(true);

    uv_run(loop, UV_RUN_DEFAULT);
}

void KernelBase::handleEvent(const SDL_Event &evt)
{
    m_world.handleEvent(evt);
}

VOLCANO_VM_END
