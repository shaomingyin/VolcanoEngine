#include <cmath>
#include <stdexcept>

#include <Volcano/Math.h>
#include <Volcano/World/SpotLight.h>

VOLCANO_WORLD_BEGIN

SpotLight::SpotLight() noexcept = default;

SpotLight::SpotLight(const Eigen::Vector3f& direction, const Eigen::Vector3f& color, float strength,
    float range, float innerAngle, float outerAngle)
    : Light(color, strength) {
    setDirection(direction);
    setRange(range);
    setAngles(innerAngle, outerAngle);
}

SpotLight::SpotLight(float dx, float dy, float dz, float r, float g, float b, float strength,
    float range, float innerAngle, float outerAngle)
    : SpotLight(Eigen::Vector3f(dx, dy, dz), Eigen::Vector3f(r, g, b), strength, range, innerAngle, outerAngle) {
}

void SpotLight::setDirection(const Eigen::Vector3f& value) {
    const float length = value.stableNorm();
    if (!value.allFinite() || !std::isfinite(length) || length == 0.0f) {
        throw std::invalid_argument("Light direction must be finite and nonzero");
    }

    direction_ = value / length;
}

void SpotLight::setAngles(float innerAngle, float outerAngle) {
    if (!std::isfinite(innerAngle) || !std::isfinite(outerAngle) ||
        innerAngle < 0.0f || innerAngle >= outerAngle || outerAngle > 90.0f ||
        std::cos(Eigen::radians(innerAngle)) <= std::cos(Eigen::radians(outerAngle))) {
        throw std::invalid_argument("Spot half-angles require distinct cones with 0 <= inner < outer <= 90 degrees");
    }
    inner_angle_ = innerAngle;
    outer_angle_ = outerAngle;
}

VOLCANO_WORLD_END
