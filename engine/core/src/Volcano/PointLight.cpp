//
//
#include <Volcano/PointLight.hpp>

VOLCANO_BEGIN

PointLight::PointLight(QObject *parent):
    Light(parent),
    m_radius(1.0f)
{
}

PointLight::~PointLight(void)
{
}

float PointLight::radius(void) const
{
    return m_radius;
}

void PointLight::setRadius(float v)
{
    if (!qFuzzyCompare(m_radius, v)) {
        m_radius = v;
        radiusChanged(v);
    }
}

VOLCANO_END
