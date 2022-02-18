//
//
#include <Volcano/Graphics/DirectionalLight.hpp>

VOLCANO_GRAPHICS_BEGIN

DirectionalLight::DirectionalLight(void)
{
}

DirectionalLight::~DirectionalLight(void)
{
}

const QVector3D &DirectionalLight::direction(void) const
{
    return m_direction;
}

void DirectionalLight::setDirection(const QVector3D &v)
{
    m_direction = v;
}

VOLCANO_GRAPHICS_END
