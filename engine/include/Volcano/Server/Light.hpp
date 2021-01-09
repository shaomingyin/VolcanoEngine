//
//
#ifndef VOLCANO_SERVER_LIGHT_HPP
#define VOLCANO_SERVER_LIGHT_HPP

#include <Volcano/Rect.hpp>
#include <Volcano/Server/Common.hpp>
#include <Volcano/Server/Object.hpp>

VOLCANO_SERVER_BEGIN

class Light: public Object {
public:
    Light(void);
    ~Light(void) override;

public:
    float strength(void) const;
    void setStrength(float v);
    const glm::vec3 &color(void) const;
    void setColor(const glm::vec3 r);

private:
    float m_strength;
    glm::vec3 m_color;
};

VOLCANO_INLINE float Light::strength(void) const
{
    return m_strength;
}

VOLCANO_INLINE void Light::setStrength(float v)
{
    m_strength = glm::clamp(v, 0.0f, 1.0f);
}

VOLCANO_INLINE const glm::vec3 &Light::color(void) const
{
    return m_color;
}

VOLCANO_INLINE void Light::setColor(const glm::vec3 r)
{
    m_color = r;
}

VOLCANO_SERVER_END

#endif // VOLCANO_SERVER_LIGHT_HPP
