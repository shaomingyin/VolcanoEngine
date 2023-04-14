//
//
#include <Volcano/Graphics/Context.hpp>

VOLCANO_GRAPHICS_BEGIN

Context::Context(Game::World &gameWorld, QObject *parent)
    : QObject(parent)
    , m_gameWorld(gameWorld)
{
}

Context::~Context(void)
{
}

bool Context::Init(void)
{
    connect(&m_gameWorld, &Game::World::sceneAdded, this, &Context::onGameWorldSceneAdded);
    connect(&m_gameWorld, &Game::World::sceneRemoved, this, &Context::onGameWorldSceneRemoved);

    return true;
}

void Context::Update(Duration elapsed)
{
}

const Game::World &Context::gameWorld(void) const
{
    return m_gameWorld;
}

void Context::onGameWorldSceneAdded(Game::Scene *p)
{

}

void Context::onGameWorldSceneRemoved(Game::Scene *p)
{

}

VOLCANO_GRAPHICS_END
