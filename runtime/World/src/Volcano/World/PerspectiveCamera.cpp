//
//
#include <Volcano/World/PerspectiveCamera.h>

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

Math::Matrix4f PerspectiveCamera::toMatrix() const {
    return Math::Matrix4f::perspective(Math::radians(fov_), aspect_, zRange().x, zRange().y);
}

VOLCANO_WORLD_END
