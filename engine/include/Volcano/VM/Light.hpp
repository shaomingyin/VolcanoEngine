//
//
#ifndef VOLCANO_VM_LIGHT_HPP
#define VOLCANO_VM_LIGHT_HPP

#include <Volcano/Rect.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Object.hpp>

VOLCANO_VM_BEGIN

class Light: public Object {
public:
    Light(void);
    ~Light(void) override;

public:
    void applyToView(Graphics::View &view) override;
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

VOLCANO_VM_END

#endif // VOLCANO_VM_LIGHT_HPP
