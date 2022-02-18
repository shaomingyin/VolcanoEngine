//
//
#include <Volcano/Graphics/PointLight.hpp>

VOLCANO_GRAPHICS_BEGIN

PointLight::PointLight(void):
    m_radius(1.0f)
{
}

PointLight::~PointLight(void)
{
}

const QVector3D &PointLight::position(void) const
{
    return m_position;
}

void PointLight::setPosition(const QVector3D &v)
{
    m_position = v;
}

float PointLight::radius(void) const
{
    return m_radius;
}

void PointLight::setRadius(float v)
{
    m_radius = v;
}

VOLCANO_GRAPHICS_END
