//
//
#include <Volcano/World/OrthographicCamera.h>

VOLCANO_WORLD_BEGIN

OrthographicCamera::OrthographicCamera() noexcept
    : rect_(0.0f, 1.0f, 0.0f, 1.0f) {
}

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float znear, float zfar) noexcept
    : Camera(znear, zfar)
    , rect_(left, right, bottom, top) {
}

Eigen::Matrix4f OrthographicCamera::toMatrix() const {
    const float left = rect_.x(), right = rect_.y(), bottom = rect_.z(), top = rect_.w();
    const float nearPlane = zRange().x(), farPlane = zRange().y();
    if (!rect_.allFinite() || !zRange().allFinite() ||
        left == right || bottom == top || nearPlane == farPlane) {
        throw std::invalid_argument("Orthographic bounds must be finite with nonzero extents");
    }
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    projection(0, 0) = 2.0f / (right - left);
    projection(1, 1) = 2.0f / (top - bottom);
    projection(2, 2) = -2.0f / (farPlane - nearPlane);
    projection(0, 3) = -(right + left) / (right - left);
    projection(1, 3) = -(top + bottom) / (top - bottom);
    projection(2, 3) = -(farPlane + nearPlane) / (farPlane - nearPlane);
    return projection;
}

VOLCANO_WORLD_END
