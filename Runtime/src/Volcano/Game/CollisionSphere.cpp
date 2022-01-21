//
//
#include <Volcano/Game/CollisionSphere.hpp>

VOLCANO_GAME_BEGIN

CollisionSphere::CollisionSphere(QObject *parent):
    CollisionShape(parent),
    m_radius(1.0f)
{
}

CollisionSphere::CollisionSphere(qreal radius, QObject *parent):
    CollisionShape(parent),
    m_radius(radius)
{
}

CollisionSphere::~CollisionSphere(void)
{
}

qreal CollisionSphere::radius(void) const
{
    return m_radius;
}

void CollisionSphere::setRadius(qreal v)
{
    m_radius = v;
    emit radiusChanged(v);
}

VOLCANO_GAME_END
