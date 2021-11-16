//
//
#include <Volcano/Graphics/PointLight.hpp>

VOLCANO_GRAPHICS_BEGIN

PointLight::PointLight(QObject *parent):
    Light(parent),
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
    if (!qFuzzyCompare(m_position, v)) {
        m_position = v;
        emit positionChanged(v);
    }
}

float PointLight::radius(void) const
{
    return m_radius;
}

void PointLight::setRadius(float v)
{
    if (!qFuzzyCompare(m_radius, v)) {
        m_radius = v;
        emit radiusChanged(v);
    }
}

VOLCANO_GRAPHICS_END
