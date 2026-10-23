//
//
#include <Volcano/World/DirectionalLight.h>

VOLCANO_WORLD_BEGIN

namespace {

Volcano::Math::Vector3f normalizedDirection(const Volcano::Math::Vector3f& direction) noexcept {
    const float magnitude = direction.length();
    return magnitude > 0.0f && std::isfinite(magnitude)
        ? direction / magnitude : Math::Vector3f(0.0f, -1.0f, 0.0f);
}

}

DirectionalLight::DirectionalLight() noexcept
    : direction_(0.0f, -1.0f, 0.0f) {
}

DirectionalLight::DirectionalLight(const Volcano::Math::Vector3f& direction, const Volcano::Math::Vector3f& color, float strength) noexcept
    : Light(color, strength)
    , direction_(normalizedDirection(direction)) {
}

DirectionalLight::DirectionalLight(float dx, float dy, float dz, float r, float g, float b, float strength) noexcept
    : Light(r, g, b, strength)
    , direction_(normalizedDirection(Volcano::Math::Vector3f(dx, dy, dz))) {
}

VOLCANO_WORLD_END
