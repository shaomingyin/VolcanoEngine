//
//
#include <Volcano/Editor/GameContext.hpp>

VOLCANO_EDITOR_BEGIN

GameContext::GameContext(QObject *parent):
    Game::Context(parent)
{
}

GameContext::~GameContext(void)
{
}

Duration GameContext::elapsed(void)
{
    return 0s;
}

Input::Service *GameContext::inputService(void)
{
    return nullptr;
}

Graphics::Service *GameContext::graphicsService(void)
{
    return nullptr;
}

Physics::Service *GameContext::physicsService(void)
{
    return nullptr;
}

Sound::Service *GameContext::soundService(void)
{
    return nullptr;
}

VOLCANO_EDITOR_END
