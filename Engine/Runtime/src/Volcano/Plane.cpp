//
//
#include <Volcano/Plane.hpp>

VOLCANO_BEGIN

Plane::Plane(QObject *parent):
    Shape(parent),
    m_x(0.0f),
    m_y(0.0f),
    m_z(0.0f),
    m_d(0.0f)
{
}

Plane::Plane(qreal x, qreal y, qreal z, qreal d, QObject *parent):
    Shape(parent),
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
    emit xChanged(v);
}

qreal Plane::y(void) const
{
    return m_y;
}

void Plane::setY(qreal v)
{
    m_y = v;
    emit yChanged(v);
}

qreal Plane::z(void) const
{
    return m_z;
}

void Plane::setZ(qreal v)
{
    m_z = v;
    emit zChanged(v);
}

qreal Plane::d(void) const
{
    return m_d;
}

void Plane::setD(qreal v)
{
    m_d = v;
    emit dChanged(v);
}

VOLCANO_END
