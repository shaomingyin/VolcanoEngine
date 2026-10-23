//
//
#ifndef VOLCANO_WORLD_CAMERA_H
#define VOLCANO_WORLD_CAMERA_H

#include <Volcano/Math/Math.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Transform.h>

VOLCANO_WORLD_BEGIN

class Camera {
public:
    Camera() noexcept;
    Camera(float znear, float zfar) noexcept;
    Camera(const Camera&) noexcept = default;
    Camera(Camera&&) noexcept = default;

public:
    Camera& operator=(const Camera&) noexcept = default;
    Camera& operator=(Camera&&) noexcept = default;

    Transform& offset() noexcept {
        return offset_;
    }

    const Transform& offset() const noexcept {
        return offset_;
    }

    Volcano::Math::Vector2f& zRange() noexcept {
        return zrange_;
    }

    const Volcano::Math::Vector2f& zRange() const noexcept {
        return zrange_;
    }

	virtual Volcano::Math::Matrix4f toMatrix() const = 0;

private:
    Transform offset_;
    Volcano::Math::Vector2f zrange_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAMERA_H
