//
//
#include <Volcano/Graphics/OrthographicCamera.h>

VOLCANO_GRAPHICS_BEGIN

OrthographicCamera::OrthographicCamera() noexcept
    : rect_(0.0f, 1.0f, 0.0f, 1.0f) {
}

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float znear, float zfar) noexcept
    : Camera(znear, zfar)
    , rect_(left, right, bottom, top) {
}

Eigen::Matrix4f OrthographicCamera::toMatrix() const noexcept {
    Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
    float znear = zRange().x();
    float zfar = zRange().y();
    m(0, 0) = 2.0f / (rect_[1] - rect_[0]);
    m(1, 1) = 2.0f / (rect_[3] - rect_[2]);
    m(2, 2) = -2.0f / (zfar - znear);
    m(0, 3) = -(rect_[1] + rect_[0]) / (rect_[1] - rect_[0]);
    m(1, 3) = -(rect_[3] + rect_[2]) / (rect_[3] - rect_[2]);
    m(2, 3) = -(zfar + znear) / (zfar - znear);
    return m;
}

void to_json(nlohmann::json& json, const OrthographicCamera& v) {
    to_json(json, static_cast<const Camera&>(v));
    json["rect"] = v.rect_;
}

void from_json(const nlohmann::json& json, OrthographicCamera& v) {
    from_json(json, static_cast<Camera&>(v));
    v.rect_ = json["rect"].get<Eigen::Vector4f>();
}

VOLCANO_GRAPHICS_END
