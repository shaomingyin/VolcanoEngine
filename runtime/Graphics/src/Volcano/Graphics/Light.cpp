//
//
#include <Volcano/Graphics/Light.h>

VOLCANO_GRAPHICS_BEGIN

Light::Light() noexcept
    : color_(1.0f, 1.0f, 1.0f)
    , strength_(1.0f) {
}

Light::Light(const Eigen::Vector3f& color, float strength) noexcept
    : color_(color)
    , strength_(strength) {
}

Light::Light(float r, float g, float b, float strength) noexcept
    : Light(Eigen::Vector3f(r, g, b), strength) {
}

void to_json(nlohmann::json& json, const Light& v) {
    json = nlohmann::json::object();
    json["color"] = v.color_;
    json["strength"] = v.strength_;
}

void from_json(const nlohmann::json& json, Light& v) {
    if (json.is_object()) {
        v.color_ = json["color"].get<Eigen::Vector3f>();
        v.strength_ = json["strength"].get<float>();
    } else {
        throw std::invalid_argument("Invalid JSON format for Volcano.Graphics.Light");
    }
}

VOLCANO_GRAPHICS_END
