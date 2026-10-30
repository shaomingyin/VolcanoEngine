#ifndef VOLCANO_WORLD_DIRECTIONALLIGHT_H
#define VOLCANO_WORLD_DIRECTIONALLIGHT_H

#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight() noexcept;
    DirectionalLight(const Eigen::Vector3f& direction, const Eigen::Vector3f& color, float strength = 1.0f);
    DirectionalLight(float dx, float dy, float dz, float r, float g, float b, float strength = 1.0f);

    constexpr const Eigen::Vector3f& direction() const noexcept {
        return direction_;
    }

    void setDirection(const Eigen::Vector3f& value);

private:
    Eigen::Vector3f direction_{0.0f, -1.0f, 0.0f};
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_DIRECTIONALLIGHT_H
