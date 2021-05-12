//
//
#include <Volcano/Launcher/Player.hpp>

VOLCANO_LAUNCHER_BEGIN

using namespace std::chrono;

Player::Player(FileSystem *fs):
	m_fs(fs),
	m_shoudlQuit(false)
{
	VOLCANO_ASSERT(m_fs != nullptr);
}

Player::~Player(void)
{
}

bool Player::init(void)
{
	spdlog::debug("Initializing main window...");
	if (!m_window.init()) {
		spdlog::error("Failed to init main window.");
		return EXIT_FAILURE;
	}

	m_shoudlQuit = false;
	m_last = Clock::now();
	m_fpsLast = m_last;
	m_fpsCount = 0;
	m_fps = 0;
	m_elapsedMin = 16667us;

	return true;
}

bool Player::shouldQuit(void) const
{
	return m_shoudlQuit;
}

void Player::handleEvent(const SDL_Event &evt)
{
	if (evt.type == SDL_WINDOWEVENT)
		m_window.handleEvent(evt.window);
}

TimePoint Player::update(void)
{
	auto begin = Clock::now();
	auto elapsed = begin - m_last;

	if (elapsed < m_elapsedMin)
		return begin + elapsed;

	frame(elapsed);

	m_fpsCount += 1;
	m_last = begin;

	auto end = Clock::now();
	if ((end - m_fpsLast) > 1s) {
		m_fps = m_fpsCount;
		m_fpsCount = 0;
		m_fpsLast = end;
	}

	elapsed = end - begin;
	if (elapsed < m_elapsedMin)
		return end + (m_elapsedMin - elapsed);

	return end;
}

FileSystem *Player::fs(void)
{
	return m_fs;
}

Window &Player::window(void)
{
	return m_window;
}

void Player::quit(void)
{
	m_shoudlQuit = true;
}

void Player::frame(Duration elapsed)
{
	m_window.update();
}

VOLCANO_LAUNCHER_END
