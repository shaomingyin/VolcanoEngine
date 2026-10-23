//
//
#include <Volcano/World/PointLight.h>

VOLCANO_WORLD_BEGIN

PointLight::PointLight() noexcept
    : range_(10.0f) {
}

PointLight::PointLight(const Volcano::Math::Vector3f& color, float strength, float range) noexcept
    : Light(color, strength)
    , range_(range) {
}

PointLight::PointLight(float r, float g, float b, float strength, float range) noexcept
    : Light(r, g, b, strength)
    , range_(range) {
}

VOLCANO_WORLD_END
