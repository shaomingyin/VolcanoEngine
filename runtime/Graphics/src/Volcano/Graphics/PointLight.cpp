//
//
#include <Volcano/Graphics/PointLight.h>

VOLCANO_GRAPHICS_BEGIN

PointLight::PointLight() noexcept
    : range_(10.0f) {
}

PointLight::PointLight(const Eigen::Vector3f& color, float strength, float range) noexcept
    : Light(color, strength)
    , range_(range) {
}

PointLight::PointLight(float r, float g, float b, float strength, float range) noexcept
    : Light(r, g, b, strength)
    , range_(range) {
}

VOLCANO_GRAPHICS_END
