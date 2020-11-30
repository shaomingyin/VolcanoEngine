//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Core.hpp>

VOLCANO_VM_BEGIN

Core::Core(uv_loop_t *loop):
    m_loop(loop),
    m_started(false),
    m_eventFirst(0),
    m_eventLast(0)
{
    VOLCANO_ASSERT(m_loop != nullptr);

    uv_async_init(loop, &m_renderAsync, [](uv_async_t *async) {
        auto core = reinterpret_cast<Core *>(async->data);
        if (core->m_graphicsWindow.activate()) {
            std::unique_lock locker(core->m_viewLock);
            core->m_graphicsRenderer.render(core->m_viewFlip[core->m_viewRendering]);
            core->m_graphicsWindow.deactivate();
        }
    });

    m_renderAsync.data = this;
}

Core::~Core(void)
{
    if (m_started)
        stop();
    else if (m_thread.joinable())
        m_thread.join();

    uv_close_sync(&m_renderAsync);
}

bool Core::start(const std::string &rootPath, const std::string &initPath)
{
    VOLCANO_ASSERT(!m_started);

    if (!m_graphicsWindow.init("VolcanoEngine", 800, 600))
        return false;

    VOLCANO_SCOPE_EXIT(r1) {
        m_graphicsWindow.shutdown();
    };

    if (!m_graphicsWindow.activate())
        return false;

    if (!m_graphicsRenderer.init(&m_graphicsWindow))
        return false;

    VOLCANO_SCOPE_EXIT(r3) {
        m_graphicsRenderer.shutdown();
    };

    m_graphicsWindow.deactivate();

    m_rootPath = rootPath;
    m_initPath = initPath;

    m_viewFlip[0].reset();
    m_viewFlip[1].reset();
    m_viewRendering = 0;

    m_eventFirst = 0;
    m_eventLast = 0;

    std::promise<bool> initPromise;
    auto initFuture = initPromise.get_future();

    m_thread = std::thread(&Core::main, this, &initPromise);

    m_started = initFuture.get();
    if (!m_started)
        return false;

    r1.dismiss();
    r3.dismiss();

    return true;
}

void Core::stop(void)
{
    VOLCANO_ASSERT(m_started);

    uv_async_send(&m_quitAsync);
    m_thread.join();

    m_graphicsWindow.activate();
    m_graphicsRenderer.shutdown();
    m_graphicsWindow.deactivate();

    m_graphicsWindow.shutdown();

    m_started = false;
}

void Core::postEvent(const SDL_Event &evt)
{
    if (evt.type != SDL_WINDOWEVENT) {
        m_eventQueue[m_eventLast & EVENT_QUEUE_MASK] = evt;
        m_eventLast += 1;
    } else {
        if (m_graphicsWindow.isValid())
            m_graphicsWindow.handleEvent(evt.window);
    }
}

void Core::main(std::promise<bool> *initPromise)
{
    VOLCANO_ASSERT(initPromise != nullptr);

    if (!m_fs.init(m_rootPath)) {
        initPromise->set_value(false);
        return;
    }

    if (!m_world.init()) {
        m_fs.shutdown();
        initPromise->set_value(false);
        return;
    }

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
        auto core = reinterpret_cast<Core *>(timer->data);
        auto now = uv_now(timer->loop);
        auto elapsed = now - core->m_lastFrameTime;
        core->frame(float(elapsed) / 1000.0f);
        core->m_lastFrameTime = now;
    }, 0, 16);
    frameTimer.data = this;

    run(&loop, initPromise);

    uv_close_sync(&frameTimer);
    uv_close_sync(&m_kickAsync);
    uv_close_sync(&m_quitAsync);

    uv_loop_close(&loop);

    m_world.shutdown();
    m_fs.shutdown();
}

void Core::run(uv_loop_t *loop, std::promise<bool> *initPromise)
{
    VOLCANO_ASSERT(loop != nullptr);
    VOLCANO_ASSERT(initPromise != nullptr);

    uv_update_time(loop);
    m_lastFrameTime = uv_now(loop);
    initPromise->set_value(true);

    uv_run(loop, UV_RUN_DEFAULT);
}

void Core::frame(float elapsed)
{
    /* Schedule rendering for last view. */ {
        std::unique_lock locker(m_viewLock);
        m_viewRendering = !m_viewRendering;
        uv_async_send(&m_renderAsync);
    }

    while (m_eventFirst < m_eventLast) {
        m_world.handleEvent(m_eventQueue[m_eventFirst & EVENT_QUEUE_MASK]);
        m_eventFirst += 1;
    }

    m_world.update(elapsed);

    Graphics::View &view = m_viewFlip[!m_viewRendering];
    view.reset();
    m_world.applyToView(view);
}

VOLCANO_VM_END
