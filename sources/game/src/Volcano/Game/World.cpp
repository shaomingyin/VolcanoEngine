//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(void)
{
}

World::~World(void)
{
}

void World::update(float elapsed)
{
}

const ScenePtrList World::scenes(void) const
{
    return m_scenes;
}

void World::addScene(ScenePtr scene)
{
}

void World::removeScene(ScenePtr scene)
{
}

VOLCANO_GAME_END
