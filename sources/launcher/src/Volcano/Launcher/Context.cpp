//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Launcher/Context.hpp>

VOLCANO_LAUNCHER_BEGIN

Context::Context(void) :
    m_window(nullptr),
    m_gl(nullptr)
{
}

Context::~Context(void)
{
    if (gl3wProcs == &m_gl3w)
        gl3wProcs = nullptr;

    if (m_gl != nullptr)
        SDL_GL_DeleteContext(m_gl);

    if (m_window != nullptr)
        SDL_DestroyWindow(m_window);
}

bool Context::init(void)
{
    VOLCANO_ASSERT(m_window == nullptr);

    SDL_GL_SetSwapInterval(1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

#ifdef VOLCANO_DEBUG
    int glFlags;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &glFlags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glFlags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    m_window = SDL_CreateWindow("Volcano", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (m_window == nullptr)
        return false;

    ScopeGuard windowGuard([=] { SDL_DestroyWindow(m_window); m_window = nullptr; });

    m_windowVisible = true;
    m_windowId = SDL_GetWindowID(m_window);

    m_gl = SDL_GL_CreateContext(m_window);
    if (m_gl == nullptr)
        return false;

    ScopeGuard glGuard([=] { SDL_GL_DeleteContext(m_gl); m_gl = nullptr; });

    SDL_GL_MakeCurrent(m_window, m_gl);

    if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) != GL3W_OK)
        return false;

    gl3wProcs = &m_gl3w;

    if (!m_graphicsRenderer.init(800, 600))
        return false;

    glGuard.dismiss();
    windowGuard.dismiss();

	return true;
}

void Context::handleEvent(const SDL_Event &event)
{
    switch (event.type) {
    case SDL_WINDOWEVENT:
        handleWindowEvent(event.window);
        break;
    default:
        break;
    }
}

void Context::frame(float elapsed)
{
    VOLCANO_ASSERT(m_window != nullptr);
    VOLCANO_ASSERT(m_gl != nullptr);

    if (!m_windowVisible)
        return;

    gl3wProcs = &m_gl3w;

    if (SDL_GL_MakeCurrent(m_window, m_gl) == 0) {
        m_graphicsRenderer.beginFrame();
        m_graphicsRenderer.endFrame();
        SDL_GL_SwapWindow(m_window);
    }
}

bool Context::canQuit(void)
{
    return true;
}

void Context::handleWindowEvent(const SDL_WindowEvent &event)
{
    VOLCANO_ASSERT(m_window != nullptr);

    if (event.windowID != m_windowId)
        return;

    switch (event.event) {
    case SDL_WINDOW_SHOWN:
        m_windowVisible = true;
        break;
    case SDL_WINDOW_HIDDEN:
        m_windowVisible = false;
        break;
    default:
        break;
    }
}

VOLCANO_LAUNCHER_END
