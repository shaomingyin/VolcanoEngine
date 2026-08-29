//
//
#include <Volcano/World/PerspectiveCamera.h>

#include <cmath>

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

Eigen::Matrix4f PerspectiveCamera::toMatrix() const noexcept {
    Eigen::Matrix4f m = Eigen::Matrix4f::Zero();
    float znear = zRange().x();
    float zfar = zRange().y();
    constexpr float Pi = 3.14159265358979323846f;
    float f = 1.0f / std::tan(fov_ * Pi / 360.0f);
    m(0, 0) = f / aspect_;
    m(1, 1) = f;
    m(2, 2) = -(zfar + znear) / (zfar - znear);
    m(2, 3) = -(2.0f * zfar * znear) / (zfar - znear);
    m(3, 2) = -1.0f;
    return m;
}

VOLCANO_WORLD_END
