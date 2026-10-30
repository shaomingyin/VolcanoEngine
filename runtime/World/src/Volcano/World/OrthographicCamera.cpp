//
//
#include <stdexcept>

#include <Volcano/World/OrthographicCamera.h>

VOLCANO_WORLD_BEGIN

OrthographicCamera::OrthographicCamera() noexcept = default;

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float znear, float zfar)
    : Camera(znear, zfar) {
    setRect(left, right, bottom, top);
}

void OrthographicCamera::setRect(float left, float right, float bottom, float top) {
    const Eigen::Vector4f value(left, right, bottom, top);
    if (!value.allFinite() || left == right || bottom == top) {
        throw std::invalid_argument("Orthographic bounds must be finite with nonzero extents");
    }
    rect_ = value;
}

Eigen::Matrix4f OrthographicCamera::toMatrix() const {
    const float left = rect_.x(), right = rect_.y(), bottom = rect_.z(), top = rect_.w();
    const float nearPlane = zRange().x(), farPlane = zRange().y();
    VOLCANO_ASSERT(rect_.allFinite() && zRange().allFinite() &&
        left != right && bottom != top && nearPlane != farPlane);
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    projection(0, 0) = 2.0f / (right - left);
    projection(1, 1) = 2.0f / (top - bottom);
    projection(2, 2) = -2.0f / (farPlane - nearPlane);
    projection(0, 3) = -(right + left) / (right - left);
    projection(1, 3) = -(top + bottom) / (top - bottom);
    projection(2, 3) = -(farPlane + nearPlane) / (farPlane - nearPlane);
    if (!projection.allFinite()) {
        throw std::invalid_argument("Orthographic projection is not finite");
    }
    return projection;
}

VOLCANO_WORLD_END
