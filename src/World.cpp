//
//
#include <Volcano/World.hpp>

VOLCANO_BEGIN

World::World(QObject *parent):
    QObject(parent),
    m_gravity(0, -9.8, 0)
{
}

World::~World(void)
{
}

void World::setGravity(const QVector3D &v)
{
    if (m_gravity != v)
    {
        m_gravity = v;
        gravityChanged(v);
    }
}

VOLCANO_END
