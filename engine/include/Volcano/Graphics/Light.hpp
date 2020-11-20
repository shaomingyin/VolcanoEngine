//
//
#ifndef VOLCANO_GRAPHICS_LIGHT_HPP
#define VOLCANO_GRAPHICS_LIGHT_HPP

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Light {
    RTTR_ENABLE()

public:
    Light(void);
    virtual ~Light(void);

public:
    const glm::vec3 &color(void) const;
    void setColor(const glm::vec3 &r);
    float strength(void) const;
    void setStrength(float v);

private:
    glm::vec3 m_color;
    float m_strength;
};

VOLCANO_INLINE const glm::vec3 &Light::color(void) const
{
    return m_color;
}

VOLCANO_INLINE void Light::setColor(const glm::vec3 &r)
{
    m_color = r;
}

VOLCANO_INLINE float Light::strength(void) const
{
    return m_strength;
}

VOLCANO_INLINE void Light::setStrength(float v)
{
    m_strength = v;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_LIGHT_HPP
