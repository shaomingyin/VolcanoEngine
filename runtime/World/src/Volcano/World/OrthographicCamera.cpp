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

Math::Matrix4f OrthographicCamera::toMatrix() const {
    return Math::Matrix4f::ortho(rect_.x, rect_.y, rect_.z, rect_.w, zRange().x, zRange().y);
}

VOLCANO_WORLD_END
