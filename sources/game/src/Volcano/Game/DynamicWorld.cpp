//
//
#include <Volcano/Game/DynamicWorld.hpp>

VOLCANO_GAME_BEGIN

DynamicWorld::DynamicWorld(void):
	m_btWorld(nullptr)
{
}

DynamicWorld::~DynamicWorld(void)
{
	if (m_btWorld)
		delete m_btWorld;
}

void DynamicWorld::update(float elapsed)
{
	World::update(elapsed);

	if (m_btWorld != nullptr) {
		m_btWorld->stepSimulation(elapsed);
		// TODO
	}
}

VOLCANO_GAME_END
