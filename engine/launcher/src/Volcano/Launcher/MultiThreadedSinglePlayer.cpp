//
//
#include <chrono>
#include <memory>

#include <Volcano/Game/World.hpp>
#include <Volcano/Launcher/MultiThreadedSinglePlayer.hpp>

VOLCANO_LAUNCHER_BEGIN

using namespace std::chrono;

MultiThreadedSinglePlayer::MultiThreadedSinglePlayer(FileSystem *fs):
	Player(fs)
{
}

MultiThreadedSinglePlayer::~MultiThreadedSinglePlayer(void)
{
}

bool MultiThreadedSinglePlayer::init(void)
{
	if (!Player::init())
		return false;

	std::promise<bool> initPromise;
	auto initResult = initPromise.get_future();
	m_thread = std::thread(&MultiThreadedSinglePlayer::threadMain, this, &initPromise);
	auto ret = initResult.wait_for(15s);
	if (ret != std::future_status::ready) {
		spdlog::error("Failed to init game thread.");
		if (m_thread.joinable())
			m_thread.join();
		return false;
	}

	return initResult.get();
}

void MultiThreadedSinglePlayer::frame(Duration elapsed)
{
	Player::frame(elapsed);
}

void MultiThreadedSinglePlayer::threadMain(std::promise<bool> *initPromise)
{
	VOLCANO_ASSERT(initPromise != nullptr);

	auto gameWorld = std::make_unique<Game::World>();
	if (!gameWorld) {
		spdlog::error("Failed to create game world object.");
		initPromise->set_value(false);
		return;
	}


}

VOLCANO_LAUNCHER_END
