//
//
#include <Volcano/Physics/Cylinder.hpp>

VOLCANO_PHYSICS_BEGIN

Cylinder::Cylinder(void):
    Sphere(1.0f),
    m_height(1.0f)
{
}

Cylinder::Cylinder(qreal radius, qreal height):
    Sphere(radius),
    m_height(height)
{
}

Cylinder::~Cylinder(void)
{
}

qreal Cylinder::height(void) const
{
    return m_height;
}

void Cylinder::setHeight(qreal v)
{
    m_height = v;
}

VOLCANO_PHYSICS_END
