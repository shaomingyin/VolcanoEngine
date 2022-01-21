//
//
#include <Volcano/Game/CollisionPlane.hpp>

VOLCANO_GAME_BEGIN

CollisionPlane::CollisionPlane(QObject *parent):
    CollisionShape(parent),
    m_x(0.0f),
    m_y(0.0f),
    m_z(0.0f),
    m_d(0.0f)
{
}

CollisionPlane::CollisionPlane(qreal x, qreal y, qreal z, qreal d, QObject *parent):
    CollisionShape(parent),
    m_x(x),
    m_y(y),
    m_z(z),
    m_d(d)
{
}

CollisionPlane::~CollisionPlane(void)
{
}

qreal CollisionPlane::x(void) const
{
    return m_x;
}

void CollisionPlane::setX(qreal v)
{
    m_x = v;
    emit xChanged(v);
}

qreal CollisionPlane::y(void) const
{
    return m_y;
}

void CollisionPlane::setY(qreal v)
{
    m_y = v;
    emit yChanged(v);
}

qreal CollisionPlane::z(void) const
{
    return m_z;
}

void CollisionPlane::setZ(qreal v)
{
    m_z = v;
    emit zChanged(v);
}

qreal CollisionPlane::d(void) const
{
    return m_d;
}

void CollisionPlane::setD(qreal v)
{
    m_d = v;
    emit dChanged(v);
}

VOLCANO_GAME_END
