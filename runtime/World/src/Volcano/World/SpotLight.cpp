//
//
#include <Volcano/World/SpotLight.h>

VOLCANO_WORLD_BEGIN

SpotLight::SpotLight() noexcept
    : range_(10.0f)
    , inner_angle_(15.0f)
    , outer_angle_(30.0f) {
}

SpotLight::SpotLight(const Volcano::Math::Vector3f& direction, const Volcano::Math::Vector3f& color, float strength, float range, float inner_angle, float outer_angle) noexcept
    : DirectionalLight(direction, color, strength)
    , range_(range)
    , inner_angle_(inner_angle)
    , outer_angle_(outer_angle) {
}

SpotLight::SpotLight(float dx, float dy, float dz, float r, float g, float b, float strength, float range, float inner_angle, float outer_angle) noexcept
    : DirectionalLight(dx, dy, dz, r, g, b, strength)
    , range_(range)
    , inner_angle_(inner_angle)
    , outer_angle_(outer_angle) {
}

VOLCANO_WORLD_END
