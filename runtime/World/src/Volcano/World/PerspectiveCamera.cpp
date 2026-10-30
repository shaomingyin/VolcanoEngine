//
//
#include <cmath>
#include <stdexcept>

#include <Volcano/Math.h>
#include <Volcano/World/PerspectiveCamera.h>

VOLCANO_WORLD_BEGIN

PerspectiveCamera::PerspectiveCamera() noexcept = default;

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float znear, float zfar) {
    setFov(fov);
    setAspect(aspect);
    setZRange(znear, zfar);
}

void PerspectiveCamera::setZRange(float znear, float zfar) {
    if (!(znear > 0.0f && zfar > znear)) {
        throw std::invalid_argument("Perspective requires 0 < near < far");
    }
    Camera::setZRange(znear, zfar);
}

Eigen::Matrix4f PerspectiveCamera::toMatrix() const {
    const float nearPlane = zRange().x(), farPlane = zRange().y();
    VOLCANO_ASSERT(std::isfinite(fov_) && std::isfinite(aspect_) &&
        std::isfinite(nearPlane) && std::isfinite(farPlane) &&
        fov_ > 0.0f && fov_ < 180.0f && aspect_ > 0.0f &&
        nearPlane > 0.0f && farPlane > nearPlane);
    const float cotangent = 1.0f / std::tan(Eigen::radians(fov_) / 2.0f);
    const float depth = farPlane - nearPlane;
    Eigen::Matrix4f projection = Eigen::Matrix4f::Zero();
    projection(0, 0) = cotangent / aspect_;
    projection(1, 1) = cotangent;
    projection(2, 2) = -(farPlane + nearPlane) / depth;
    projection(3, 2) = -1.0f;
    projection(2, 3) = -2.0f * nearPlane * farPlane / depth;
    if (!projection.allFinite()) {
        throw std::invalid_argument("Perspective projection is not finite");
    }
    return projection;
}

VOLCANO_WORLD_END
