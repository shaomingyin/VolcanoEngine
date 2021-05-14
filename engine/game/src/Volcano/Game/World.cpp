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

bool World::init(void)
{
	return true;
}

void World::update(float elapsed)
{
	if (m_btWorld != nullptr)
		m_btWorld->stepSimulation(elapsed);
}

const Eigen::Vector3f &World::gravity(void) const
{
	return m_gravity;
}

void World::setGravity(const Eigen::Vector3f &v)
{
	m_gravity = v;

	if (m_btWorld != nullptr)
		m_btWorld->setGravity(btVector3(v[0], v[1], v[2]));
}

VOLCANO_GAME_END
