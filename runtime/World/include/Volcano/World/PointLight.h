#ifndef VOLCANO_WORLD_POINTLIGHT_H
#define VOLCANO_WORLD_POINTLIGHT_H

#include <cmath>
#include <stdexcept>

#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class PointLight: public Light {
public:
    PointLight() noexcept;
    PointLight(const Eigen::Vector3f& color, float strength, float range);
    PointLight(float r, float g, float b, float strength, float range);

    constexpr float range() const noexcept {
        return range_;
    }

    constexpr void setRange(float value) {
        if (!std::isfinite(value) || value <= 0.0f) {
            throw std::invalid_argument("Point light range must be finite and positive");
        }

        range_ = value;
    }

private:
    float range_ = 10.0f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_POINTLIGHT_H
