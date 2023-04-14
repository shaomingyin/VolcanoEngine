//
//
#include <Volcano/Game/DynamicWorld.hpp>

VOLCANO_GAME_BEGIN

DynamicWorld::DynamicWorld(QObject *parent)
    : World(parent)
{
}

DynamicWorld::~DynamicWorld(void)
{
}

const QVector3D &DynamicWorld::gravity(void) const
{
    return m_gravity;
}

void DynamicWorld::setGravity(const QVector3D &v)
{
    if (!qFuzzyCompare(m_gravity, v)) {
        m_gravity = v;
        emit gravityChanged(v);
    }
}

VOLCANO_GAME_END
