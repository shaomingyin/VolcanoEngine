#include <cmath>
#include <stdexcept>

#include <Volcano/World/DirectionalLight.h>

VOLCANO_WORLD_BEGIN

DirectionalLight::DirectionalLight() noexcept = default;

DirectionalLight::DirectionalLight(const Eigen::Vector3f& direction, const Eigen::Vector3f& color, float strength)
    : Light(color, strength) {
    setDirection(direction);
}

DirectionalLight::DirectionalLight(float dx, float dy, float dz, float r, float g, float b, float strength)
    : DirectionalLight(Eigen::Vector3f(dx, dy, dz), Eigen::Vector3f(r, g, b), strength) {
}

void DirectionalLight::setDirection(const Eigen::Vector3f& value) {
    const float length = value.stableNorm();
    if (!value.allFinite() || !std::isfinite(length) || length == 0.0f) {
        throw std::invalid_argument("Light direction must be finite and nonzero");
    }

    direction_ = value / length;
}

VOLCANO_WORLD_END
