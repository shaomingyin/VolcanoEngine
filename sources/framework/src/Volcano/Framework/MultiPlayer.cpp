//
//
#include <Volcano/Framework/MultiPlayer.hpp>

VOLCANO_FRAMEWORK_BEGIN

MultiPlayer::MultiPlayer(void)
{
}

MultiPlayer::~MultiPlayer(void)
{
}

bool MultiPlayer::init(void)
{
    if (!Client::init())
        return false;

    return true;
}

void MultiPlayer::frame(float elapsed)
{
    Client::frame(elapsed);
}

VOLCANO_FRAMEWORK_END
