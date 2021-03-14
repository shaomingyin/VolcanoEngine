//
//
#ifndef VOLCANO_GAME_LIGHT_HPP
#define VOLCANO_GAME_LIGHT_HPP

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Light {
public:
    Light(void);
    virtual ~Light(void);

public:
    float strength(void) const;
    void setStrength(float v);
    const Eigen::Vector3f &color(void) const;
    void setColor(const Eigen::Vector3f &r);

private:
    float m_strength;
    Eigen::Vector3f m_color;
};

VOLCANO_INLINE float Light::strength(void) const
{
    return m_strength;
}

VOLCANO_INLINE void Light::setStrength(float v)
{
    m_strength = std::clamp(v, 0.0f, 1.0f);
}

VOLCANO_INLINE const Eigen::Vector3f &Light::color(void) const
{
    return m_color;
}

VOLCANO_INLINE void Light::setColor(const Eigen::Vector3f &r)
{
    m_color = r;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_HPP
