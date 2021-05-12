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
	m_ecsWorld.component<Position>();
	m_ecsWorld.component<Scale>();
	m_ecsWorld.component<Rotation>();
	m_ecsWorld.component<Velocity>();

	return true;
}

void World::update(float elapsed)
{
	if (m_btWorld != nullptr)
		m_btWorld->stepSimulation(elapsed);

	m_ecsWorld.progress(elapsed);
}

flecs::world &World::ecsWorld(void)
{
	return m_ecsWorld;
}

const flecs::world &World::ecsWorld(void) const
{
	return m_ecsWorld;
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

void World::onAdd(flecs::entity ent, Position, Scale, Rotation)
{
}

VOLCANO_GAME_END
