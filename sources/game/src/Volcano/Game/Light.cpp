//
//
#include <algorithm>

#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

Light::Light(void)
{
}

Light::~Light(void)
{
}

float Light::strength(void) const
{
    return m_strength;
}

void Light::setStrength(float v)
{
    m_strength = std::clamp(v, 0.0f, 1.0f);
}

const Eigen::Vector3f &Light::color(void) const
{
    return m_color;
}

void Light::setColor(const Eigen::Vector3f &r)
{
    m_color = r;
}

VOLCANO_GAME_END
