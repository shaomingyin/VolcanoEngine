#include <cmath>
#include <stdexcept>

#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

Camera::Camera() noexcept = default;

Camera::Camera(float znear, float zfar) {
    setZRange(znear, zfar);
}

void Camera::setZRange(float znear, float zfar) {
    if (!std::isfinite(znear) || !std::isfinite(zfar) || znear == zfar) {
        throw std::invalid_argument("Camera clipping planes must be finite and distinct");
    }
    zrange_ = {znear, zfar};
}

void Camera::setOutputSize(Size value) {
    if (value.width < 0 || value.height < 0 || ((value.width == 0) != (value.height == 0))) {
        throw std::invalid_argument("Camera output size must be empty or have positive dimensions");
    }
    output_size_ = value;
}

VOLCANO_WORLD_END
