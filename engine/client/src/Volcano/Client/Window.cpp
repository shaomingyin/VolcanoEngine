//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Client/Window.hpp>

VOLCANO_CLIENT_BEGIN

Window::Window(const Napi::CallbackInfo &info):
	Napi::ObjectWrap<Window>(info),
	m_window(nullptr)
{
}

Window::~Window(void)
{
	if (m_window != nullptr)
		glfwDestroyWindow(m_window);
}

void Window::initExports(Napi::Env env, Napi::Object exports)
{
	Napi::Function constructor = DefineClass(env, "Window", {
		InstanceAccessor<&Window::isVisible, &Window::setVisible>("visible"),
		InstanceAccessor<&Window::isFullScreen, &Window::setFullScreen>("fullScreen"),
		InstanceAccessor<&Window::title, &Window::setTitle>("title"),
		InstanceAccessor<&Window::position, &Window::setPosition>("position"),
		InstanceAccessor<&Window::size, &Window::setSize>("size"),
		InstanceMethod<&Window::init>("init")
	});

	exports.Set("Window", constructor);
}

Napi::Value Window::init(const Napi::CallbackInfo &info)
{
	if (m_window != nullptr)
		return Napi::Error::New(info.Env(), "Already initialized.").Value();

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
		// TODO arg error
		break;
	}

	spdlog::info("Creating OpenGL window...");

	GLFWmonitor *defaultMonitor = nullptr;
	if (isFullScreen) {
		defaultMonitor = glfwGetPrimaryMonitor();
		if (defaultMonitor == nullptr)
			return Napi::Error::New(info.Env(), "Can not get default monitor.").Value();
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

	m_window = glfwCreateWindow(m_rect[2], m_rect[3], m_title.c_str(), defaultMonitor, nullptr);
	if (m_window == nullptr)
		return Napi::Error::New(info.Env(), "Failed to create OpenGL window.").Value();

	ScopeGuard windowGuard([this] { glfwDestroyWindow(m_window); m_window = nullptr; });

	glfwShowWindow(m_window);
	m_visible = true;

	glfwGetWindowPos(m_window, &m_rect[0], &m_rect[1]);

	int windowWidth, windowHeight;
	glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
	m_rect[2] = windowWidth;
	m_rect[3] = windowHeight;

	glfwSetWindowPosCallback(m_window, &Window::onPos);
	glfwSetWindowSizeCallback(m_window, &Window::onSize);
	glfwSetKeyCallback(m_window, &Window::onKey);
	glfwSetMouseButtonCallback(m_window, &Window::onMouseButton);
	glfwSetCursorPosCallback(m_window, &Window::onMouseMove);
	glfwSetScrollCallback(m_window, &Window::onScroll);

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#endif

	glfwSetWindowUserPointer(m_window, this);

	glfwMakeContextCurrent(m_window);

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)glfwGetProcAddress) != GL3W_OK)
		return Napi::Error::New(info.Env(), "Failed to initialize GL3W.").Value();

	gl3wProcs = &m_gl3w;

	spdlog::info("Initializing renderer...");
	if (!m_renderer.init(windowWidth, windowHeight))
		return Napi::Error::New(info.Env(), "Failed to initialize renderer.").Value();

	windowGuard.dismiss();

	return Napi::Value::From(info.Env(), true);
}

Napi::Value Window::connect(const Napi::CallbackInfo &info)
{
	std::string ip;
	int port = 7788;

	switch (info.Length()) {
	case 2:
		port = info[1].ToNumber().Int32Value();
	case 1:
		ip = info[0].ToString();
		break;
	default:
		return Napi::Error::New(info.Env(), "Invalid parameter(s).").Value();
		break;
	}

	napi_status ns;
	uv_loop_t *loop = nullptr;

	ns = napi_get_uv_event_loop(info.Env(), &loop);
	if (ns != napi_ok || loop == nullptr) 
		return Napi::Error::New(info.Env(), "Failed to get uv loop.").Value();

	uv_udp_init(loop, &m_connection);

	sockaddr_in addr;
	uv_ip4_addr(ip.c_str(), port, &addr);
	uv_udp_connect(&m_connection, (sockaddr *)&addr);
	uv_udp_recv_start(&m_connection, &Window::connectionAllocCallback, &Window::connectionReceiveCallback);
	m_connection.data = this;

	return Napi::Value::From(info.Env(), true);
}

Napi::Value Window::isVisible(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_window != nullptr && m_visible);
}

void Window::setVisible(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (m_window == nullptr)
		return;

	if (value.ToBoolean())
		glfwShowWindow(m_window);
	else
		glfwHideWindow(m_window);
}

Napi::Value Window::isFullScreen(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(),
		m_window != nullptr && glfwGetWindowMonitor(m_window) != nullptr);
}

void Window::setFullScreen(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (m_window == nullptr)
		return;

	auto monitor = glfwGetWindowMonitor(m_window);

	if (value.ToBoolean()) {
		if (monitor == nullptr) {
			glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, m_rect[2], m_rect[3], GLFW_DONT_CARE);
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	} else {
		if (monitor != nullptr) {
			glfwSetWindowMonitor(m_window, nullptr, 100, 100, m_rect[2], m_rect[3], GLFW_DONT_CARE);
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

Napi::Value Window::title(const Napi::CallbackInfo &info)
{
	if (m_window == nullptr)
		return Napi::Error::New(info.Env(), "Not initialized.").Value();

	return Napi::Value::From(info.Env(), m_title);
}

void Window::setTitle(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (m_window == nullptr)
		return;

	m_title = value.ToString();
	glfwSetWindowTitle(m_window, m_title.c_str());
}

Napi::Value Window::position(const Napi::CallbackInfo &info)
{
	if (m_window == nullptr) {
		// TODO exception?
	}

	auto value = Napi::Array::New(info.Env());

	value[0u] = m_rect[0];
	value[1u] = m_rect[1];

	return value;
}

void Window::setPosition(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (m_window == nullptr) {
		// TODO exception?
	}

	if (!value.IsObject()) {
		// TODO bad arg
	}

	auto object = value.ToObject();

	glfwSetWindowPos(m_window,
		object.Get(0u).ToNumber().Int32Value(),
		object.Get(1u).ToNumber().Int32Value());
}

Napi::Value Window::size(const Napi::CallbackInfo &info)
{
	if (m_window == nullptr) {
		// TODO exception?
	}

	if (m_window == nullptr) {
		// TODO exception?
	}

	auto value = Napi::Array::New(info.Env());

	value[0u] = m_rect[2];
	value[1u] = m_rect[3];

	return value;
}

void Window::setSize(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (m_window == nullptr) {
		// TODO exception?
	}

	if (!value.IsArray()) {
		// TODO bad arg
	}

	auto object = value.ToObject();

	glfwSetWindowSize(m_window,
		object.Get(0u).ToNumber().Int32Value(),
		object.Get(1u).ToNumber().Int32Value());
}

void Window::update(void)
{
	if (!m_visible || m_rect[2] < 1 || m_rect[3] < 1)
		return;

	glfwMakeContextCurrent(m_window);
	gl3wProcs = &m_gl3w;
	// m_renderer.update();
	glfwSwapBuffers(m_window);
}

void Window::onPos(GLFWwindow *window, int x, int y)
{
	auto p = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		p->m_rect[0] = x;
		p->m_rect[1] = y;
	}
}

void Window::onSize(GLFWwindow *window, int width, int height)
{
	auto p = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		p->m_rect[2] = width;
		p->m_rect[3] = height;
	}
}

void Window::onKey(GLFWwindow *window, int keyCode, int scanCode, int action, int mods)
{
	auto p = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (p == nullptr)
		return;

	//if (keyCode == GLFW_KEY_ENTER && action == GLFW_PRESS && mods & GLFW_MOD_ALT)
	//	p->toggleFullScreen();
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

VOLCANO_CLIENT_END
