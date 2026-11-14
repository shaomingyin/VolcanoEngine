#include <cmath>
#include <stdexcept>

#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

Camera::Camera() noexcept = default;

Camera::Camera(float znear, float zfar) {
    setZRange(znear, zfar);
}

Camera::~Camera() = default;

Camera::Camera(Camera&&) noexcept = default;

Camera& Camera::operator=(Camera&&) noexcept = default;

void Camera::setZRange(float znear, float zfar) {
    if (!std::isfinite(znear) || !std::isfinite(zfar) || znear == zfar) {
        throw std::invalid_argument("Camera clipping planes must be finite and distinct");
    }
    zrange_ = {znear, zfar};
}

VOLCANO_WORLD_END
