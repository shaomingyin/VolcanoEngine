//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    QObject(parent),
    m_gravity(0.0f, -9.8f, 0.0f),
    m_btWorld(nullptr)
{
}

World::~World(void)
{
    if (m_btWorld != nullptr)
        delete m_btWorld;
}

bool World::init(int flags)
{
    Q_ASSERT(m_btWorld == nullptr);

    if (flags & InitPhysics)
    {

    }

    return true;
}

void World::setGravity(const QVector3D &r)
{
    if (m_gravity == r)
        return;

    m_gravity = r;

    if (m_btWorld != nullptr)
        m_btWorld->setGravity(btVector3(r.x(), r.y(), r.z()));

    gravityChanged(r);
}

void World::stepSimulation(float elapsed)
{
    if (m_btWorld != nullptr)
    {
        m_btWorld->stepSimulation(elapsed);

        // TODO
    }
}

VOLCANO_GAME_END
