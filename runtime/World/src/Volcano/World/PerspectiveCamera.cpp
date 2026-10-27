//
//
#include <cmath>

#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/Math.h>

VOLCANO_WORLD_BEGIN

PerspectiveCamera::PerspectiveCamera() noexcept
    : fov_(90.0f)
    , aspect_(1.0f) {
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float znear, float zfar) noexcept
    : Camera(znear, zfar)
    , fov_(fov)
    , aspect_(aspect) {
}

Eigen::Matrix4f PerspectiveCamera::toMatrix() const {
    const float nearPlane = zRange().x(), farPlane = zRange().y();
    if (!std::isfinite(fov_) || !std::isfinite(aspect_) ||
        !std::isfinite(nearPlane) || !std::isfinite(farPlane) ||
        !(fov_ > 0.0f && fov_ < 180.0f) || !(aspect_ > 0.0f) ||
        !(nearPlane > 0.0f && farPlane > nearPlane)) {
        throw std::invalid_argument("Perspective requires 0 < fov < 180, aspect > 0 and 0 < near < far");
    }
    const float cotangent = 1.0f / std::tan(Eigen::radians(fov_) / 2.0f);
    const float depth = farPlane - nearPlane;
    Eigen::Matrix4f projection = Eigen::Matrix4f::Zero();
    projection(0, 0) = cotangent / aspect_;
    projection(1, 1) = cotangent;
    projection(2, 2) = -(farPlane + nearPlane) / depth;
    projection(3, 2) = -1.0f;
    projection(2, 3) = -2.0f * nearPlane * farPlane / depth;
    return projection;
}

VOLCANO_WORLD_END
