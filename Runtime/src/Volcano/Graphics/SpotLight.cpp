//
//
#include <Volcano/Graphics/SpotLight.hpp>

#ifdef VOLCANO_GRAPHICS

VOLCANO_GRAPHICS_BEGIN

SpotLight::SpotLight(void):
    m_distance(1.0f)
{
}

SpotLight::~SpotLight(void)
{
}

const QVector3D &SpotLight::position(void) const
{
    return m_position;
}

void SpotLight::setPosition(const QVector3D &v)
{
    m_position = v;
}

qreal SpotLight::distance(void) const
{
    return m_distance;
}

void SpotLight::setDistance(qreal v)
{
    m_distance = v;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS
