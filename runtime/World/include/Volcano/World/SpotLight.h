#ifndef VOLCANO_WORLD_SPOTLIGHT_H
#define VOLCANO_WORLD_SPOTLIGHT_H

#include <cmath>
#include <stdexcept>

#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class SpotLight: public Light {
public:
    SpotLight() noexcept;
    SpotLight(const Eigen::Vector3f& direction, const Eigen::Vector3f& color, float strength,
        float range, float innerAngle, float outerAngle);
    SpotLight(float dx, float dy, float dz, float r, float g, float b, float strength,
        float range, float innerAngle, float outerAngle);

    constexpr const Eigen::Vector3f& direction() const noexcept {
        return direction_;
    }

    void setDirection(const Eigen::Vector3f& value);

    constexpr float range() const noexcept {
        return range_;
    }

    constexpr void setRange(float value) {
        if (!std::isfinite(value) || value <= 0.0f) {
            throw std::invalid_argument("Spot light range must be finite and positive");
        }

        range_ = value;
    }

    constexpr float innerAngle() const noexcept {
        return inner_angle_;
    }

    constexpr float outerAngle() const noexcept {
        return outer_angle_;
    }

    void setAngles(float innerAngle, float outerAngle);

private:
    Eigen::Vector3f direction_{0.0f, -1.0f, 0.0f};
    float range_ = 10.0f;
    float inner_angle_ = 15.0f;
    float outer_angle_ = 30.0f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SPOTLIGHT_H
