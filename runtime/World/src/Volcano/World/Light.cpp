#include <cmath>
#include <stdexcept>

#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

void Light::setColor(const Eigen::Vector3f& value) {
    if (!value.allFinite() || (value.array() < 0.0f).any() || !(value * strength_).allFinite()) {
        throw std::invalid_argument("Light color and resulting radiance must be finite and nonnegative");
    }
    color_ = value;
}

void Light::setStrength(float value) {
    if (!std::isfinite(value) || value < 0.0f) {
        throw std::invalid_argument("Light strength must be finite and nonnegative");
    }
    if (!(color_ * value).allFinite()) {
        throw std::invalid_argument("Light radiance must be finite");
    }

    strength_ = value;
}

Light::Light() noexcept = default;

Light::Light(const Eigen::Vector3f& color, float strength) {
    setStrength(strength);
    setColor(color);
}

Light::Light(float r, float g, float b, float strength)
    : Light(Eigen::Vector3f(r, g, b), strength) {
}

VOLCANO_WORLD_END
