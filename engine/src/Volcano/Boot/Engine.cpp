//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Boot/Engine.hpp>

VOLCANO_BOOT_BEGIN

Engine::Engine(uv_loop_t *loop):
    VM::Kernel(loop)
{
    uv_async_init(loop, &m_renderAsync, [](uv_async_t *async) {
        auto engine = reinterpret_cast<Engine *>(async->data);
        std::unique_lock locker(engine->m_viewLock);
        const Graphics::View &view = engine->m_viewFlip[engine->m_viewRendering];
        engine->m_renderer.render(view);
    });

    m_renderAsync.data = this;
}

Engine::~Engine(void)
{
    uv_close_sync(&m_renderAsync);
}

bool Engine::init(void)
{
    if (!VM::Kernel::init())
        return false;

    VOLCANO_SCOPE_EXIT(r1) {
        VM::Kernel::shutdown();
    };

    if (!m_window.init("VolcanoEngine", 800, 600))
        return false;

    VOLCANO_SCOPE_EXIT(r2) {
        m_window.shutdown();
    };

    if (!m_renderer.init(&m_window))
        return false;

    VOLCANO_SCOPE_EXIT(r3) {
        m_renderer.shutdown();
    };

    m_viewFlip[0].reset();
    m_viewFlip[1].reset();
    m_viewRendering = 0;

    r1.dismiss();
    r2.dismiss();
    r3.dismiss();

    return true;
}

void Engine::shutdown(void)
{
    m_renderer.shutdown();
    m_window.shutdown();

    VM::Kernel::shutdown();
}

void Engine::initExports(VM::Registration &reg)
{
    VM::Kernel::initExports(reg);
}

void Engine::frame(float elapsed)
{
    {
        std::unique_lock locker(m_viewLock);
        m_viewRendering = !m_viewRendering;
        uv_async_send(&m_renderAsync);
    }

    VM::Kernel::frame(elapsed);

    Graphics::View &view = m_viewFlip[!m_viewRendering];
    view.reset();
    world().applyToView(view);
}

void Engine::handleEvent(const SDL_Event &evt)
{
    if (evt.type != SDL_WINDOWEVENT)
        VM::Kernel::handleEvent(evt);
    else
        m_window.handleEvent(evt.window);
}

VOLCANO_BOOT_END
