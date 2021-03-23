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

const CameraPtrList World::cameras(void) const
{
    return m_cameras;
}

const ScenePtrList World::scenes(void) const
{
    return m_scenes;
}

void World::addCamera(CameraPtr camera)
{
}

void World::removeCamera(CameraPtr camera)
{
}

void World::addScene(ScenePtr scene)
{
}

void World::removeScene(ScenePtr scene)
{
}

VOLCANO_GAME_END
