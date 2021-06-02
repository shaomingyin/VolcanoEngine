//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Graphics/Window.hpp>

VOLCANO_GRAPHICS_BEGIN

Window::Window(const Napi::CallbackInfo &info):
	Napi::ObjectWrap<Window>(info),
	m_handle(nullptr),
	m_isVisible(false),
	m_isGrabMouse(false)
{
	m_title = "Volcano";
	m_rect[0] = 0;
	m_rect[1] = 0;
	m_rect[2] = 800;
	m_rect[3] = 600;
	bool isFullScreen = false;

	switch (info.Length()) {
	case 6:
		isFullScreen = info[5].ToBoolean();
	case 5:
		m_rect[0] = info[3].ToNumber().Int32Value();
		m_rect[1] = info[4].ToNumber().Int32Value();
	case 3:
		m_rect[2] = info[1].ToNumber().Int32Value();
		m_rect[3] = info[2].ToNumber().Int32Value();
	case 1:
		m_title = info[0].ToString();
	case 0:
		break;
	default:
		Napi::ThrowError(info.Env(), "Invalid prameter(s).");
		return;
	}

	spdlog::info("Get monitor informations...");

	GLFWmonitor *defaultMonitor = nullptr;
	if (isFullScreen) {
		defaultMonitor = glfwGetPrimaryMonitor();
		if (defaultMonitor == nullptr) {
			Napi::ThrowError(info.Env(), "Failed to get default monitor.");
			return;
		}
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

#ifdef VOLCANO_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

	spdlog::info("Creating OpenGL window...");

	m_handle = glfwCreateWindow(m_rect[2], m_rect[3], m_title.c_str(), defaultMonitor, nullptr);
	if (m_handle == nullptr) {
		Napi::ThrowError(info.Env(), "Failed to initialize OpenGL window.");
		return;
	}

	ScopeGuard windowGuard([this] { glfwDestroyWindow(m_handle); m_handle = nullptr; });

	m_isVisible = false;

	glfwGetWindowPos(m_handle, &m_rect[0], &m_rect[1]);

	int windowWidth, windowHeight;
	glfwGetWindowSize(m_handle, &windowWidth, &windowHeight);
	m_rect[2] = windowWidth;
	m_rect[3] = windowHeight;

	glfwSetWindowPosCallback(m_handle, &Window::onPos);
	glfwSetWindowSizeCallback(m_handle, &Window::onSize);
	glfwSetKeyCallback(m_handle, &Window::onKey);
	glfwSetMouseButtonCallback(m_handle, &Window::onMouseButton);
	glfwSetCursorPosCallback(m_handle, &Window::onMouseMove);
	glfwSetScrollCallback(m_handle, &Window::onScroll);

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#endif

	glfwSetWindowUserPointer(m_handle, this);

	glfwMakeContextCurrent(m_handle);

	spdlog::info("Initializing GL3W...");

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)glfwGetProcAddress) != GL3W_OK) {
		Napi::ThrowError(info.Env(), "Failed to initialize GL3W.");
		return;
	}

	gl3wProcs = &m_gl3w;

	spdlog::info("Initializing renderer...");

	auto renderer = Renderer::newInstance({});
	if (renderer.IsEmpty() || renderer.IsNull()) {
		Napi::ThrowError(info.Env(), "Failed to create renderer object.");
		return;
	}

	m_rendererRef = Napi::ObjectReference::New(renderer, 2);
	m_renderer = Renderer::Unwrap(renderer);
	if (m_renderer == nullptr || !m_renderer->init(windowWidth, windowHeight)) {
		Napi::ThrowError(info.Env(), "Failed to initialize renderer.");
		return;
	}

	setFpsMax(60);

	windowGuard.dismiss();
}

Window::~Window(void)
{
	m_rendererRef.Reset();

	if (m_handle != nullptr)
		glfwDestroyWindow(m_handle);
}

Napi::Function Window::defineClass(Napi::Env env)
{
	return DefineClass(env, "Window", {
		InstanceAccessor<&Window::renderer>("renderer"),
		InstanceAccessor<&Window::isVisible, &Window::setVisible>("visible"),
		InstanceAccessor<&Window::isFullScreen, &Window::setFullScreen>("fullScreen"),
		InstanceAccessor<&Window::title, &Window::setTitle>("title"),
		InstanceAccessor<&Window::position, &Window::setPosition>("position"),
		InstanceAccessor<&Window::size, &Window::setSize>("size"),
		InstanceAccessor<&Window::isGrabMouse, &Window::setGrabMouse>("grabMouse"),
		InstanceAccessor<&Window::fps>("fps"),
		InstanceAccessor<&Window::fpsMax, &Window::setFpsMax>("fpsMax")
	});
}

Napi::Value Window::renderer(const Napi::CallbackInfo &info)
{
	return m_rendererRef.Value();
}

Napi::Value Window::isVisible(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_handle != nullptr && m_isVisible);
}

void Window::setVisible(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (m_handle == nullptr)
		return;

	if (value.ToBoolean())
		glfwShowWindow(m_handle);
	else
		glfwHideWindow(m_handle);
}

Napi::Value Window::isFullScreen(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(),
		m_handle != nullptr && glfwGetWindowMonitor(m_handle) != nullptr);
}

void Window::setFullScreen(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (m_handle == nullptr)
		return;

	auto monitor = glfwGetWindowMonitor(m_handle);

	if (value.ToBoolean()) {
		if (monitor == nullptr)
			glfwSetWindowMonitor(m_handle, glfwGetPrimaryMonitor(), 0, 0, m_rect[2], m_rect[3], GLFW_DONT_CARE);
	} else {
		if (monitor != nullptr)
			glfwSetWindowMonitor(m_handle, nullptr, 100, 100, m_rect[2], m_rect[3], GLFW_DONT_CARE);
	}
}

Napi::Value Window::title(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_title);
}

void Window::setTitle(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	m_title = value.ToString();

	if (m_handle != nullptr)
		glfwSetWindowTitle(m_handle, m_title.c_str());
}

Napi::Value Window::position(const Napi::CallbackInfo &info)
{
	if (m_handle == nullptr) {
		// TODO exception?
	}

	auto value = Napi::Array::New(info.Env());

	value[0u] = m_rect[0];
	value[1u] = m_rect[1];

	return value;
}

void Window::setPosition(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (m_handle == nullptr)
		return;

	if (!value.IsArray()) {
		// TODO bad arg
	}

	auto object = value.ToObject();

	glfwSetWindowPos(m_handle,
		object.Get(0u).ToNumber().Int32Value(),
		object.Get(1u).ToNumber().Int32Value());
}

Napi::Value Window::size(const Napi::CallbackInfo &info)
{
	auto value = Napi::Array::New(info.Env());

	value[0u] = m_rect[2];
	value[1u] = m_rect[3];

	return value;
}

void Window::setSize(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (m_handle == nullptr) {
		// TODO exception?
	}

	if (!value.IsArray()) {
		// TODO bad arg
	}

	auto object = value.ToObject();

	glfwSetWindowSize(m_handle,
		object.Get(0u).ToNumber().Int32Value(),
		object.Get(1u).ToNumber().Int32Value());
}

Napi::Value Window::isGrabMouse(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_isGrabMouse);
}

void Window::setGrabMouse(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	m_isGrabMouse = value.ToBoolean();
	if (m_isGrabMouse)
		glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

Napi::Value Window::fps(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_frameCountPerSecond);
}

Napi::Value Window::fpsMax(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), 1000000000ns / m_elapsedMin);
}

void Window::setFpsMax(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	setFpsMax(value.ToNumber().Int32Value());
}

void Window::setFpsMax(int v)
{
	int tmp = v;
	if (tmp < 1)
		tmp = 1;

	m_elapsedMin = 1000000000ns / v;

	m_frameCount = 0;
	m_frameCountPerSecond = 0;
	m_frameCountLast = Clock::now();
}

void Window::update(void)
{
	VOLCANO_ASSERT(m_handle != nullptr);

	if (m_isVisible && m_rect[2] > 0 && m_rect[3] > 0 && m_renderer != nullptr) {
		glfwMakeContextCurrent(m_handle);
		// TODO
		glfwSwapBuffers(m_handle);
	}
}

void Window::onPos(GLFWwindow *window, int x, int y)
{
	auto p = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		p->m_rect[0] = x;
		p->m_rect[1] = y;
		p->onPositionChanged(x, y);
	}
}

void Window::onSize(GLFWwindow *window, int width, int height)
{
	auto p = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		p->m_rect[2] = width;
		p->m_rect[3] = height;
		p->onSizeChanged(width, height);
	}
}

void Window::onKey(GLFWwindow *window, int keyCode, int scanCode, int action, int mods)
{
	auto p = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		// TODO
	}
}

void Window::onMouseButton(GLFWwindow *window, int button, int action, int mods)
{
	auto p = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		// TODO
	}
}

void Window::onMouseMove(GLFWwindow *window, double x, double y)
{
	auto p = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		// TODO
	}
}

void Window::onScroll(GLFWwindow *window, double x, double y)
{
	auto p = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		// TODO
	}
}

void Window::connectionAllocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{

}

void Window::connectionReceiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
{

}

VOLCANO_GRAPHICS_END
