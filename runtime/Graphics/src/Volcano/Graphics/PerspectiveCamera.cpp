//
//
#include <Volcano/Graphics/PerspectiveCamera.h>

VOLCANO_GRAPHICS_BEGIN

PerspectiveCamera::PerspectiveCamera() noexcept
    : fov_(90.0f)
    , aspect_(1.0f) {
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float znear, float zfar) noexcept
    : Camera(znear, zfar)
    , fov_(fov)
    , aspect_(aspect) {
}

Eigen::Matrix4f PerspectiveCamera::toMatrix() const noexcept {
    Eigen::Matrix4f m = Eigen::Matrix4f::Zero();
    float znear = zRange().x();
    float zfar = zRange().y();
    float f = 1.0f / std::tan(fov_ * 0.5f);
    m(0, 0) = f / aspect_;
    m(1, 1) = f;
    m(2, 2) = -(zfar + znear) / (zfar - znear);
    m(2, 3) = -(2.0f * zfar * znear) / (zfar - znear);
    m(3, 2) = -1.0f;
    return m;
}

void to_json(nlohmann::json& json, const PerspectiveCamera& v) {
    to_json(json, static_cast<const Camera&>(v));
    json["fov"] = v.fov_;
    json["aspect"] = v.aspect_;
}

void from_json(const nlohmann::json& json, PerspectiveCamera& v) {
    from_json(json, static_cast<Camera&>(v));
    v.fov_ = json["fov"].get<float>();
    v.aspect_ = json["aspect"].get<float>();
}

VOLCANO_GRAPHICS_END
