//
//
#include <Volcano/Launcher/SinglePlayer.hpp>

VOLCANO_LAUNCHER_BEGIN

SinglePlayer::SinglePlayer(FileSystem *fs):
	Player(fs),
	m_gameWorld(nullptr)
{
}

SinglePlayer::~SinglePlayer(void)
{
	if (m_gameWorld != nullptr)
		delete m_gameWorld;
}

bool SinglePlayer::init(void)
{
	VOLCANO_ASSERT(m_gameWorld == nullptr);

	if (!Player::init())
		return false;

	// TODO create game world.

	return true;
}

void SinglePlayer::handleEvent(const SDL_Event &evt)
{
	Player::handleEvent(evt);
}

void SinglePlayer::frame(Duration elapsed)
{
	//VOLCANO_ASSERT(m_gameWorld != nullptr);

	// TODO update game world.

	Player::frame(elapsed);
}

VOLCANO_LAUNCHER_END
