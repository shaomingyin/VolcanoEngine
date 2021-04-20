//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/System/Engine.hpp>

VOLCANO_SYSTEM_BEGIN

Engine::Engine(uv_loop_t *loop, std::string_view rootPath):
    m_window(nullptr),
    m_gl(nullptr),
	m_vmKernel(loop, rootPath)
{
	VOLCANO_ASSERT(loop != nullptr);
}

Engine::~Engine(void)
{
	if (m_window != nullptr)
		stop();
}

bool Engine::start(void)
{
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
	VOLCANO_LOGI("Create OpenGL window...");

	m_window = SDL_CreateWindow("Volcano",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (m_window == nullptr) {
		VOLCANO_LOGE("Failed to create OpenGL window.");
		return false;
	}

	ScopeGuard windowGuard([=] {
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	});

	m_windowID = SDL_GetWindowID(m_window);
	m_windowVisible = true;

	SDL_GetWindowSize(m_window, &m_windowSize[0], &m_windowSize[1]);

	VOLCANO_ASSERT(m_gl == nullptr);
	VOLCANO_LOGI("Creating OpenGL context...");

	m_gl = SDL_GL_CreateContext(m_window);
	if (m_gl == nullptr) {
		VOLCANO_LOGE("Failed to create OpenGL context.");
		return false;
	}

	ScopeGuard glGuard([=] {
		SDL_GL_DeleteContext(m_gl);
		m_gl = nullptr;
	});

	if (SDL_GL_MakeCurrent(m_window, m_gl) < 0) {
		VOLCANO_LOGE("Failed to make current OpenGL context.");
		return false;
	}

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) != GL3W_OK) {
		VOLCANO_LOGE("Failed to init GL3W.");
		return false;
	}

	gl3wProcs = &m_gl3w;

	// TODO init graphics, input, sound subsystems...

	VOLCANO_LOGI("Create VM...");

	if (!m_vmKernel.start()) {
		VOLCANO_LOGE("Failed to start VM.");
		return false;
	}

	glGuard.dismiss();
	windowGuard.dismiss();

	return true;
}

void Engine::stop(void)
{
	VOLCANO_LOGI("Shuting down VM...");
    m_vmKernel.stop();

	VOLCANO_LOGI("Destroying OpenGL context...");
	VOLCANO_ASSERT(m_gl != nullptr);
	SDL_GL_DeleteContext(m_gl);
	m_gl = nullptr;

	VOLCANO_LOGI("Destroying OpenGL window...");
	VOLCANO_ASSERT(m_window != nullptr);
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
}

void Engine::handleEvent(const SDL_Event &event)
{
    switch (event.type) {
    case SDL_WINDOWEVENT:
        handleWindowEvent(event.window);
        break;
    default:
        m_vmKernel.postEvent(event);
        break;
    }
}

void Engine::handleWindowEvent(const SDL_WindowEvent &event)
{
	VOLCANO_ASSERT(m_window != nullptr);

    if (event.windowID != m_windowID)
        return;

    switch (event.event) {
    case SDL_WINDOWEVENT_SHOWN:
        m_windowVisible = true;
        break;
    case SDL_WINDOWEVENT_HIDDEN:
        m_windowVisible = false;
        break;
    case SDL_WINDOWEVENT_RESIZED:
        m_windowSize[0] = event.data1;
        m_windowSize[1] = event.data2;
        break;
    default:
        break;
    }
}

VOLCANO_SYSTEM_END
