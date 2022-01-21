//
//
#include <Volcano/Physics/Plane.hpp>

VOLCANO_PHYSICS_BEGIN

Plane::Plane(void):
    m_x(0.0f),
    m_y(0.0f),
    m_z(0.0f),
    m_d(0.0f)
{
}

Plane::Plane(qreal x, qreal y, qreal z, qreal d):
    m_x(x),
    m_y(y),
    m_z(z),
    m_d(d)
{
}

Plane::~Plane(void)
{
}

qreal Plane::x(void) const
{
    return m_x;
}

void Plane::setX(qreal v)
{
    m_x = v;
}

qreal Plane::y(void) const
{
    return m_y;
}

void Plane::setY(qreal v)
{
    m_y = v;
}

qreal Plane::z(void) const
{
    return m_z;
}

void Plane::setZ(qreal v)
{
    m_z = v;
}

qreal Plane::d(void) const
{
    return m_d;
}

void Plane::setD(qreal v)
{
    m_d = v;
}

VOLCANO_PHYSICS_END
