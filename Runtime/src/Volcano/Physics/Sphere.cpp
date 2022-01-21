//
//
#include <Volcano/Physics/Sphere.hpp>

VOLCANO_PHYSICS_BEGIN

Sphere::Sphere(void):
    m_radius(1.0f)
{
}

Sphere::Sphere(qreal radius):
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
}

VOLCANO_PHYSICS_END
