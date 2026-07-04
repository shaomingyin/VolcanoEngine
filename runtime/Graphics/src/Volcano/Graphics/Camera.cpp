//
//
#include <Volcano/Graphics/Camera.h>

VOLCANO_GRAPHICS_BEGIN

Camera::Camera() noexcept
    : Camera(0.1f, 100.0f) {
}

Camera::Camera(float znear, float zfar) noexcept
    : zrange_(znear, zfar) {
}

void to_json(nlohmann::json& json, const Camera& v) {
    json = nlohmann::json::object();
    if (!v.offset_.isIdentity()) {
        json["offset"] = v.offset_;
    }
    json["zrange"] = v.zrange_;
}

void from_json(const nlohmann::json& json, Camera& v) {
    if (json.is_object()) {
        auto offset_it = json.find("offset");
        if (offset_it != json.end()) {
            v.offset_ = offset_it->get<Transform>();
        } else {
            v.offset_.reset();
        }
        v.zrange_ = json["zrange"].get<Eigen::Vector2f>();
    } else {
        throw std::invalid_argument("Invalid JSON format for Camera");
    }
}

VOLCANO_GRAPHICS_END
