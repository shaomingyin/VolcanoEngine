//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/System/Window.hpp>

VOLCANO_SYSTEM_BEGIN

Window::Window(void) :
	m_window(nullptr),
	m_gl(nullptr)
{
}

Window::~Window(void)
{
	if (m_window != nullptr)
		shutdown();
}

bool Window::init(std::string_view title, int width, int height)
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

	ScopeGuard windowGuard([this] { SDL_DestroyWindow(m_window); m_window = nullptr; });

	m_id = SDL_GetWindowID(m_window);
	m_visible = true;

	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
	m_size[0] = windowWidth;
	m_size[1] = windowHeight;

	VOLCANO_ASSERT(m_gl == nullptr);
	VOLCANO_LOGI("Creating OpenGL context...");

	m_gl = SDL_GL_CreateContext(m_window);
	if (m_gl == nullptr) {
		VOLCANO_LOGE("Failed to create OpenGL context.");
		return false;
	}

	ScopeGuard glGuard([=] { SDL_GL_DeleteContext(m_gl); m_gl = nullptr; });

	if (SDL_GL_MakeCurrent(m_window, m_gl) < 0) {
		VOLCANO_LOGE("Failed to make current OpenGL context.");
		return false;
	}

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) != GL3W_OK) {
		VOLCANO_LOGE("Failed to init GL3W.");
		return false;
	}

	gl3wProcs = &m_gl3w;

#ifdef VOLCANO_DEBUG
	glDebugMessageCallback(&Window::glDebugLog, nullptr);
#endif

	VOLCANO_LOGI("Initializing renderer...");
	if (!m_renderer.init(Eigen::Vector4i(0, 0, width, height))) {
		VOLCANO_LOGE("Failed to init renderer.");
		return false;
	}

	glGuard.dismiss();
	windowGuard.dismiss();

	return true;
}

void Window::shutdown(void)
{
	if (m_window == nullptr)
		return;

	m_renderer.shutdown();

	VOLCANO_LOGI("Destroying OpenGL context...");
	VOLCANO_ASSERT(m_gl != nullptr);
	SDL_GL_DeleteContext(m_gl);
	m_gl = nullptr;

	SDL_DestroyWindow(m_window);
	m_window = nullptr;
}

void Window::update(void)
{
	VOLCANO_ASSERT(m_window != nullptr);
	VOLCANO_ASSERT(m_gl != nullptr);

	if (SDL_GL_MakeCurrent(m_window, m_gl) == 0) {
		gl3wProcs = &m_gl3w;
		m_renderer.update();
		SDL_GL_SwapWindow(m_window);
	}
}

void Window::handleEvent(const SDL_WindowEvent &event)
{
	VOLCANO_ASSERT(m_window != nullptr);

	if (event.windowID != m_id)
		return;

	switch (event.event) {
	case SDL_WINDOWEVENT_SHOWN:
		m_visible = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_visible = false;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		m_size[0] = event.data1;
		m_size[1] = event.data2;
		m_renderer.setViewport(Eigen::Vector4i(0, 0, m_size[0], m_size[1]));
		break;
	default:
		break;
	}
}

bool Window::isVisible(void) const
{
	VOLCANO_ASSERT(m_window != nullptr);

	return m_visible;
}

void Window::setVisible(bool v)
{
	VOLCANO_ASSERT(m_window != nullptr);

	if (v)
		SDL_ShowWindow(m_window);
	else
		SDL_HideWindow(m_window);
}

const Eigen::Vector2i &Window::size(void) const
{
	VOLCANO_ASSERT(m_window != nullptr);

	return m_size;
}

void Window::setSize(const Eigen::Vector2i &size)
{
	VOLCANO_ASSERT(m_window != nullptr);

	SDL_SetWindowSize(m_window, size.x(), size.y());
}

bool Window::isFullscreen(void) const
{
	VOLCANO_ASSERT(m_window != nullptr);

	return false;
}

void Window::setFullscreen(bool v)
{
	VOLCANO_ASSERT(m_window != nullptr);
}

const std::string &Window::title(void) const
{
	VOLCANO_ASSERT(m_window != nullptr);

	return m_title;
}

void Window::setTitle(std::string_view title)
{
	VOLCANO_ASSERT(m_window != nullptr);

	m_title = title;
	SDL_SetWindowTitle(m_window, title.data());
}

VM::Renderer *Window::renderer(void)
{
	VOLCANO_ASSERT(m_window != nullptr);

	return &m_renderer;
}

void Window::glDebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	SDL_LogPriority logPrio;

	const char *typeText;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		typeText = "ERROR";
		logPrio = SDL_LOG_PRIORITY_ERROR;
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		typeText = "DEPRECATED_BEHAVIOR";
		logPrio = SDL_LOG_PRIORITY_WARN;
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		typeText = "UNDEFINED_BEHAVIOR";
		logPrio = SDL_LOG_PRIORITY_ERROR;
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		typeText = "PORTABILITY";
		logPrio = SDL_LOG_PRIORITY_WARN;
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		typeText = "PERFORMANCE";
		logPrio = SDL_LOG_PRIORITY_WARN;
		break;
	case GL_DEBUG_TYPE_MARKER:
		typeText = "MARKER";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		typeText = "PUSH_GROUP";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		typeText = "POP_GROUP";
		break;
	case GL_DEBUG_TYPE_OTHER:
		typeText = "OTHER";
		break;
	default:
		typeText = "UNKNOWN";
		logPrio = SDL_LOG_PRIORITY_ERROR;
		break;
	}

	const char *severityText;

	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		severityText = "L";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		severityText = "M";
		logPrio = SDL_LOG_PRIORITY_WARN;
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		severityText = "H";
		logPrio = SDL_LOG_PRIORITY_ERROR;
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		severityText = "N";
		break;
	default:
		severityText = "?";
		logPrio = SDL_LOG_PRIORITY_WARN;
		break;
	}

	writeLog(logPrio, "[GL-%06d] %s %s: %s", id, severityText, typeText, message);
}

VOLCANO_SYSTEM_END
