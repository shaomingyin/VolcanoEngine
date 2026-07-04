//
//
#include <Volcano/Graphics/SpotLight.h>

VOLCANO_GRAPHICS_BEGIN

SpotLight::SpotLight() noexcept
    : inner_angle_(15.0f)
    , outer_angle_(30.0f) {
}

SpotLight::SpotLight(const Eigen::Vector3f& direction, const Eigen::Vector3f& color, float strength, float inner_angle, float outer_angle) noexcept
    : DirectionalLight(direction, color, strength)
    , inner_angle_(inner_angle)
    , outer_angle_(outer_angle) {
}

SpotLight::SpotLight(float dx, float dy, float dz, float r, float g, float b, float strength, float inner_angle, float outer_angle) noexcept
    : DirectionalLight(dx, dy, dz, r, g, b, strength)
    , inner_angle_(inner_angle)
    , outer_angle_(outer_angle) {
}

VOLCANO_GRAPHICS_END
