#ifndef VOLCANO_WORLD_CAMERA_H
#define VOLCANO_WORLD_CAMERA_H

#include <Eigen/Core>

#include <Volcano/Transform.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Camera {
public:
    Camera() noexcept;
    Camera(float znear, float zfar);
    virtual ~Camera() = default;

    constexpr const Transform& offset() const noexcept {
        return offset_;
    }

    void setOffset(const Transform& value) noexcept {
        offset_ = value;
    }

    constexpr const Eigen::Vector2f& zRange() const noexcept {
        return zrange_;
    }

    virtual void setZRange(float znear, float zfar);
    virtual Eigen::Matrix4f toMatrix() const = 0;

protected:
    Camera(const Camera&) = default;
    Camera(Camera&&) = default;
    Camera& operator=(const Camera&) = default;
    Camera& operator=(Camera&&) = default;

private:
    Transform offset_;
    Eigen::Vector2f zrange_{0.1f, 100.0f};
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAMERA_H
