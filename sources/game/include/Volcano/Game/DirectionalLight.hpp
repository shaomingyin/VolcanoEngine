//
//
#ifndef VOLCANO_GAME_DIRECTIONALLIGHT_HPP
#define VOLCANO_GAME_DIRECTIONALLIGHT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight(void);
    ~DirectionalLight(void) override;

public:
    const Eigen::Vector3f &direction(void) const;
    void setDirection(const Eigen::Vector3f &r);

private:
    Eigen::Vector3f m_direction;
};

VOLCANO_INLINE const Eigen::Vector3f &DirectionalLight::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void DirectionalLight::setDirection(const Eigen::Vector3f &r)
{
    m_direction = r;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DIRECTIONALLIGHT_HPP
