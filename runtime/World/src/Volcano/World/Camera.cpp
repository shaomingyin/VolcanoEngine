//
//
#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

Camera::Camera() noexcept
    : Camera(0.1f, 100.0f) {
}

Camera::Camera(float znear, float zfar) noexcept
    : zrange_(znear, zfar) {
}

VOLCANO_WORLD_END
