//
//
#ifndef VOLCANO_VM_DIRECTIONALLIGHT_HPP
#define VOLCANO_VM_DIRECTIONALLIGHT_HPP

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Light.hpp>

VOLCANO_VM_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight(void);
    ~DirectionalLight(void) override;

public:
    void applyToView(Graphics::View &view) override;
    const glm::vec3 &direction(void) const;
    void setDirection(const glm::vec3 &r);

private:
    glm::vec3 m_direction;
};

VOLCANO_INLINE const glm::vec3 &DirectionalLight::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void DirectionalLight::setDirection(const glm::vec3 &r)
{
    VOLCANO_ASSERT(glm::epsilonEqual(glm::length(r), 1.0f, 0.001f));

    m_direction = r;
}

VOLCANO_VM_END

#endif // VOLCANO_VM_DIRECTIONALLIGHT_HPP
