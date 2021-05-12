//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Launcher/Window.hpp>

VOLCANO_LAUNCHER_BEGIN

Window::Window(void):
	m_window(nullptr),
	m_glContext(nullptr)
{
}

Window::~Window(void)
{
	// TODO
}

bool Window::init(void)
{
	VOLCANO_ASSERT(m_window == nullptr);

	// OpenGL 3.3 core profile.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// R8G8B8-D16
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef VOLCANO_DEBUG
	int glFlags;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &glFlags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glFlags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	VOLCANO_ASSERT(m_window == nullptr);
	spdlog::info("Creating OpenGL window...");

	m_window = SDL_CreateWindow("Volcano",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (m_window == nullptr) {
		spdlog::error("Failed to create OpenGL window.");
		return false;
	}

	ScopeGuard windowGuard([this] { SDL_DestroyWindow(m_window); m_window = nullptr; });

	m_id = SDL_GetWindowID(m_window);
	m_visible = true;

	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
	m_size[0] = windowWidth;
	m_size[1] = windowHeight;

	VOLCANO_ASSERT(m_glContext == nullptr);
	spdlog::info("Creating OpenGL context...");

	m_glContext = SDL_GL_CreateContext(m_window);
	if (m_glContext == nullptr) {
		spdlog::error("Failed to create OpenGL context.");
		return false;
	}

	ScopeGuard glGuard([=] { SDL_GL_DeleteContext(m_glContext); m_glContext = nullptr; });

	if (SDL_GL_MakeCurrent(m_window, m_glContext) < 0) {
		spdlog::error("Failed to make current OpenGL context.");
		return false;
	}

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) != GL3W_OK) {
		spdlog::error("Failed to init GL3W.");
		return false;
	}

	gl3wProcs = &m_gl3w;

	spdlog::info("Initializing renderer...");
	if (!m_renderer.init(800, 600)) {
		spdlog::error("Failed to init renderer.");
		return false;
	}

	glGuard.dismiss();
	windowGuard.dismiss();

	return true;
}

bool Window::handleEvent(const SDL_WindowEvent &evt)
{
	VOLCANO_ASSERT(m_window != nullptr);

	if (evt.windowID != m_id)
		return false;

	switch (evt.event) {
	case SDL_WINDOWEVENT_RESIZED:
		break;
	case SDL_WINDOWEVENT_SHOWN:
		m_visible = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_visible = false;
		break;
	default:
		return false;
		break;
	}

	return true;
}

void Window::update(void)
{
	VOLCANO_ASSERT(m_window != nullptr);
	VOLCANO_ASSERT(m_glContext != nullptr);

	if (!m_visible || m_size[0] < 1 || m_size[1] < 1)
		return;

	if (SDL_GL_MakeCurrent(m_window, m_glContext) == 0) {
		// m_renderer.update();
		SDL_GL_SwapWindow(m_window);
	}
}

Graphics::Renderer &Window::renderer(void)
{
	return m_renderer;
}

VOLCANO_LAUNCHER_END
