//
//
#include <Volcano/Sphere.hpp>

VOLCANO_BEGIN

Sphere::Sphere(QObject *parent):
    Shape(parent),
    m_radius(1.0f)
{
}

Sphere::Sphere(qreal radius, QObject *parent):
    Shape(parent),
    m_radius(radius)
{
}

Sphere::~Sphere(void)
{
}

qreal Sphere::radius(void) const
{
    return m_radius;
}

void Sphere::setRadius(qreal v)
{
    m_radius = v;
    emit radiusChanged(v);
}

VOLCANO_END
