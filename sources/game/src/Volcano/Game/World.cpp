//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(void):
	m_btWorld(nullptr)
{
}

World::~World(void)
{
	if (m_btWorld)
		delete m_btWorld;
}

void World::update(float elapsed)
{
	if (m_btWorld != nullptr) {
		m_btWorld->stepSimulation(elapsed);
		// TODO
	}
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
