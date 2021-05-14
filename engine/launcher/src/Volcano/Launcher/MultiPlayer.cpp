//
//
#include <Volcano/Launcher/MultiPlayer.hpp>

VOLCANO_LAUNCHER_BEGIN

MultiPlayer::MultiPlayer(FileSystem *fs) :
	Player(fs)
{
}

MultiPlayer::~MultiPlayer(void)
{
}

bool MultiPlayer::init(void)
{
	if (!Player::init())
		return false;

	return true;
}

void MultiPlayer::frame(Duration elapsed)
{
	Player::frame(elapsed);
}

VOLCANO_LAUNCHER_END
