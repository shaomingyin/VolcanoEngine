//
//
#include <Volcano/Game/DynamicWorld.hpp>

VOLCANO_GAME_BEGIN

DynamicWorld::DynamicWorld(QObject *parent):
    World(parent),
    m_btWorld(nullptr),
    m_gravity(0.0f, -9.8f, 0.0f)
{
}

DynamicWorld::~DynamicWorld(void)
{
}

void DynamicWorld::update(float elapsed)
{
    World::update(elapsed);

    m_btWorld->stepSimulation(elapsed);

    // TODO
}

const QVector3D &DynamicWorld::gravity(void) const
{
    return m_gravity;
}

void DynamicWorld::setGravity(const QVector3D &v)
{
    if (m_gravity == v)
        return;

    m_gravity = v;

    if (m_btWorld != nullptr)
        m_btWorld->setGravity(btVector3(m_gravity[0], m_gravity[1], m_gravity[2]));

    emit gravityChanged(v);
}

VOLCANO_GAME_END
