//
//
#include <Volcano/Framework/Client.hpp>

VOLCANO_FRAMEWORK_BEGIN

Client::Client(void)
{
}

Client::~Client(void)
{
}

bool Client::init(void)
{
    if (!Game::ContextBase::init())
        return false;

    // if (m_graphicsRenderer.init)

    auto p = createInputContext();
    if (p == nullptr)
        return false;

    m_inputContext.reset(p);

    return true;
}

void Client::frame(float elapsed)
{
    VOLCANO_ASSERT(m_inputContext);
    m_inputContext->frame(elapsed);

    Game::ContextBase::frame(elapsed);
}

VOLCANO_FRAMEWORK_END
