//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Launcher/Player.hpp>

VOLCANO_LAUNCHER_BEGIN

using namespace std::chrono;

Player::Player(FileSystem *fs):
	m_fs(fs),
	m_window(nullptr),
	m_shoudlQuit(false)
{
	VOLCANO_ASSERT(m_fs != nullptr);
}

Player::~Player(void)
{
	if (m_window != nullptr)
		glfwDestroyWindow(m_window);
}

bool Player::init(void)
{
	VOLCANO_ASSERT(m_window == nullptr);
	spdlog::info("Creating OpenGL window...");

	m_window = glfwCreateWindow(800, 600, "Volcano", nullptr, nullptr);
	if (m_window == nullptr) {
		spdlog::error("Failed to create OpenGL window.");
		return false;
	}

	ScopeGuard windowGuard([this] { glfwDestroyWindow(m_window); m_window = nullptr; });

	glfwShowWindow(m_window);
	m_visible = true;

	int windowWidth, windowHeight;
	glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
	m_size[0] = windowWidth;
	m_size[1] = windowHeight;

	glfwSetWindowSizeCallback(m_window, &Player::onSize);
	glfwSetKeyCallback(m_window, &Player::onKey);
	glfwSetMouseButtonCallback(m_window, &Player::onMouseButton);
	glfwSetCursorPosCallback(m_window, &Player::onMouseMove);

	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#endif

	glfwSetWindowUserPointer(m_window, this);

	glfwMakeContextCurrent(m_window);

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)glfwGetProcAddress) != GL3W_OK) {
		spdlog::error("Failed to init GL3W.");
		return false;
	}

	gl3wProcs = &m_gl3w;

	spdlog::info("Initializing renderer...");
	if (!m_renderer.init(windowWidth, windowHeight)) {
		spdlog::error("Failed to init renderer.");
		return false;
	}

	m_shoudlQuit = false;
	m_last = Clock::now();
	m_fpsLast = m_last;
	m_fpsCount = 0;
	m_fps = 0;
	m_elapsedMin = 16667us;

	windowGuard.dismiss();

	return true;
}

TimePoint Player::update(void)
{
	VOLCANO_ASSERT(m_window != nullptr);
	m_shoudlQuit = glfwWindowShouldClose(m_window);

	auto begin = Clock::now();
	auto elapsed = begin - m_last;

	if (elapsed < m_elapsedMin)
		return begin + elapsed;

	frame(elapsed);

	m_fpsCount += 1;
	m_last = begin;

	auto end = Clock::now();
	if ((end - m_fpsLast) >= 1s) {
		m_fps = m_fpsCount;
		m_fpsCount = 0;
		m_fpsLast = end;
		spdlog::debug("fps {}", m_fps);
	}

	elapsed = end - begin;
	if (elapsed < m_elapsedMin)
		return end + (m_elapsedMin - elapsed);

	return end;
}

bool Player::shouldQuit(void) const
{
	return m_shoudlQuit;
}

void Player::frame(Duration elapsed)
{
	VOLCANO_ASSERT(m_window != nullptr);

	if (!m_visible || m_size[0] < 1 || m_size[1] < 1)
		return;

	glfwMakeContextCurrent(m_window);
	gl3wProcs = &m_gl3w;
	// m_renderer.update();
	glfwSwapBuffers(m_window);
}

void Player::onSize(GLFWwindow *window, int width, int height)
{
	spdlog::debug("size {} {}", width, height);
	auto p = reinterpret_cast<Player *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		// TODO
	}
}

void Player::onKey(GLFWwindow *window, int keyCode, int scanCode, int action, int mods)
{
	spdlog::debug("key {}", keyCode);
	auto p = reinterpret_cast<Player *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		// TODO
	}
}

void Player::onMouseButton(GLFWwindow *window, int button, int action, int mods)
{
	spdlog::debug("btn {}", button);
	auto p = reinterpret_cast<Player *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		// TODO
	}
}

void Player::onMouseMove(GLFWwindow *window, double x, double y)
{
	spdlog::debug("mm {} {}", x, y);
	auto p = reinterpret_cast<Player *>(glfwGetWindowUserPointer(window));
	if (p != nullptr) {
		// TODO
	}
}

VOLCANO_LAUNCHER_END
