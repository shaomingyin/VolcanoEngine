#include <Volcano/World/PointLight.h>

VOLCANO_WORLD_BEGIN

PointLight::PointLight() noexcept = default;

PointLight::PointLight(const Eigen::Vector3f& color, float strength, float range)
    : Light(color, strength) {
    setRange(range);
}

PointLight::PointLight(float r, float g, float b, float strength, float range)
    : PointLight(Eigen::Vector3f(r, g, b), strength, range) {
}

VOLCANO_WORLD_END
