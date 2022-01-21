//
//
#include <Volcano/Game/CollisionCylinder.hpp>

VOLCANO_GAME_BEGIN

CollisionCylinder::CollisionCylinder(QObject *parent):
    CollisionSphere(parent),
    m_height(1.0f)
{
}

CollisionCylinder::CollisionCylinder(qreal radius, qreal height, QObject *parent):
    CollisionSphere(radius, parent),
    m_height(height)
{
}

CollisionCylinder::~CollisionCylinder(void)
{
}

qreal CollisionCylinder::height(void) const
{
    return m_height;
}

void CollisionCylinder::setHeight(qreal v)
{
    m_height = v;
    emit heightChanged(v);
}

VOLCANO_GAME_END
