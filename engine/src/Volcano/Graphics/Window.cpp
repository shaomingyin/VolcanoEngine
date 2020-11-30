//
//
#include <Volcano/Graphics/Window.hpp>

VOLCANO_GRAPHICS_BEGIN

Window::Window(void):
	m_handle(nullptr),
	m_id(0),
    m_glContext(nullptr)
{
}

Window::~Window(void)
{
    shutdown();
}

bool Window::init(const char *name, int width, int height, int flags)
{
	VOLCANO_ASSERT(m_handle == nullptr);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef VOLCANO_DEBUG
	int gl_flags;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &gl_flags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, gl_flags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    m_handle = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
        flags | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (m_handle == nullptr)
		goto bad0;

	m_id = SDL_GetWindowID(m_handle);
	m_visible = true;

	{
		int width, height;
		SDL_GetWindowSize(m_handle, &width, &height);
		resize(width, height);
	}

    VOLCANO_ASSERT(m_glContext == nullptr);
    m_glContext = SDL_GL_CreateContext(m_handle);
    if (m_glContext == nullptr)
		goto bad1;

    if (SDL_GL_MakeCurrent(m_handle, m_glContext) < 0)
		goto bad2;

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) != GL3W_OK)
		goto bad2;

	gl3wProcs = &m_gl3w;

	return true;

bad2:
    SDL_GL_DeleteContext(m_glContext);
    m_glContext = nullptr;

bad1:
	SDL_DestroyWindow(m_handle);
	m_handle = nullptr;

bad0:
	return false;
}

void Window::shutdown(void)
{
    if (m_glContext != nullptr) {
        SDL_GL_DeleteContext(m_glContext);
        m_glContext = nullptr;
    }

    if (m_handle != nullptr) {
        SDL_DestroyWindow(m_handle);
        m_handle = nullptr;
    }
}

bool Window::handleEvent(const SDL_WindowEvent &evt)
{
    VOLCANO_ASSERT(m_handle != nullptr);

	if (evt.windowID != m_id)
        return false;

    bool ret;

	switch (evt.event) {
	case SDL_WINDOWEVENT_SHOWN:
		m_visible = true;
        ret = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_visible = false;
        ret = true;
        break;
	case SDL_WINDOWEVENT_RESIZED:
        Surface::resize(evt.data1, evt.data2);
        ret = true;
		break;
    default:
        ret = false;
        break;
	}

    return ret;
}

bool Window::isValid(void)
{
    return (m_handle != nullptr);
}

void Window::resize(int width, int height)
{
    VOLCANO_ASSERT(m_handle != nullptr);

    SDL_SetWindowSize(m_handle, width, height);
    Surface::resize(width, height);
}

bool Window::activate(void)
{
	VOLCANO_ASSERT(m_handle != nullptr);
    VOLCANO_ASSERT(m_glContext != nullptr);

    if (width() < 1 || height() < 1)
        return false;

    if (!Surface::activate())
        return false;

    if (SDL_GL_MakeCurrent(m_handle, m_glContext) < 0) {
        Surface::deactivate();
		return false;
    }

	gl3wProcs = &m_gl3w;

    return true;
}

void Window::deactivate(void)
{
	VOLCANO_ASSERT(m_handle != nullptr);
    VOLCANO_ASSERT(m_glContext != nullptr);
    VOLCANO_ASSERT(SDL_GL_GetCurrentContext() == m_glContext);
	VOLCANO_ASSERT(gl3wProcs == &m_gl3w);

	gl3wProcs = nullptr;
	SDL_GL_SwapWindow(m_handle);
    Surface::deactivate();
}

VOLCANO_GRAPHICS_END
