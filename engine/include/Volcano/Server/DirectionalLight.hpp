//
//
#ifndef VOLCANO_SERVER_DIRECTIONALLIGHT_HPP
#define VOLCANO_SERVER_DIRECTIONALLIGHT_HPP

#include <Volcano/Server/Common.hpp>
#include <Volcano/Server/Light.hpp>

VOLCANO_SERVER_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight(void);
    ~DirectionalLight(void) override;

public:
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

VOLCANO_SERVER_END

#endif // VOLCANO_SERVER_DIRECTIONALLIGHT_HPP
