//
//
#include <Volcano/Framework/SinglePlayer.hpp>

VOLCANO_FRAMEWORK_BEGIN

SinglePlayer::SinglePlayer(void)
{
}

SinglePlayer::~SinglePlayer(void)
{
}

bool SinglePlayer::init(void)
{
    if (!Client::init())
        return false;

    // TODO init server in backend thread?

    return true;
}

void SinglePlayer::frame(float elapsed)
{
    // TODO run server in backend thread?
    m_server.frame(elapsed);

    // TODO sync with server backend thread

    Client::frame(elapsed);
}

VOLCANO_FRAMEWORK_END
