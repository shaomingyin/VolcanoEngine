//
//
#include <Volcano/Cylinder.hpp>

VOLCANO_BEGIN

Cylinder::Cylinder(QObject *parent):
    Shape(parent),
    m_radius(1.0f),
    m_height(1.0f)
{
}

Cylinder::Cylinder(qreal radius, qreal height, QObject *parent):
    Shape(parent),
    m_radius(radius),
    m_height(height)
{
}

Cylinder::~Cylinder(void)
{
}

qreal Cylinder::radius(void) const
{
    return m_radius;
}

void Cylinder::setRadius(qreal v)
{
    m_radius = v;
    emit radiusChanged(v);
}

qreal Cylinder::height(void) const
{
    return m_height;
}

void Cylinder::setHeight(qreal v)
{
    m_height = v;
    emit heightChanged(v);
}

VOLCANO_END
