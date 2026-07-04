//
//
#include <Volcano/Graphics/DirectionalLight.h>

VOLCANO_GRAPHICS_BEGIN

DirectionalLight::DirectionalLight() noexcept
    : direction_(0.0f, -1.0f, 0.0f) {
}

DirectionalLight::DirectionalLight(const Eigen::Vector3f& direction, const Eigen::Vector3f& color, float strength) noexcept
    : Light(color, strength)
    , direction_(direction.normalized()) {
}

DirectionalLight::DirectionalLight(float dx, float dy, float dz, float r, float g, float b, float strength) noexcept
    : Light(r, g, b, strength)
    , direction_(Eigen::Vector3f(dx, dy, dz).normalized()) {
}

void to_json(nlohmann::json& json, const DirectionalLight& v) {
    to_json(json, static_cast<const Light&>(v));
    json["direction"] = v.direction_;
}

void from_json(const nlohmann::json& json, DirectionalLight& v) {
    from_json(json, static_cast<Light&>(v));
    v.direction_ = json["direction"];
}

VOLCANO_GRAPHICS_END
