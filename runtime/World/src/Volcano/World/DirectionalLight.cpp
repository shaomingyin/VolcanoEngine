//
//
#include <Volcano/World/DirectionalLight.h>

VOLCANO_WORLD_BEGIN

namespace {

Eigen::Vector3f normalizedDirection(const Eigen::Vector3f& direction) noexcept {
    return direction.squaredNorm() > 0.0f ? direction.normalized() : Eigen::Vector3f(0.0f, -1.0f, 0.0f);
}

}

DirectionalLight::DirectionalLight() noexcept
    : direction_(0.0f, -1.0f, 0.0f) {
}

DirectionalLight::DirectionalLight(const Eigen::Vector3f& direction, const Eigen::Vector3f& color, float strength) noexcept
    : Light(color, strength)
    , direction_(normalizedDirection(direction)) {
}

DirectionalLight::DirectionalLight(float dx, float dy, float dz, float r, float g, float b, float strength) noexcept
    : Light(r, g, b, strength)
    , direction_(normalizedDirection(Eigen::Vector3f(dx, dy, dz))) {
}

VOLCANO_WORLD_END
