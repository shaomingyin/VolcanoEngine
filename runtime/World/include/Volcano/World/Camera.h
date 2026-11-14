#ifndef VOLCANO_WORLD_CAMERA_H
#define VOLCANO_WORLD_CAMERA_H

#include <memory>
#include <utility>

#include <Eigen/Core>

#include <Volcano/Transform.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Texture.h>

VOLCANO_WORLD_BEGIN

class Camera {
public:
    Camera() noexcept;
    Camera(float znear, float zfar);
    virtual ~Camera();

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

    const Texture* hud() const noexcept {
        return hud_.get();
    }

    Texture* hud() noexcept {
        return hud_.get();
    }

    void setHud(std::unique_ptr<Texture> value) noexcept {
        hud_ = std::move(value);
    }

    std::unique_ptr<Texture> takeHud() noexcept {
        return std::exchange(hud_, nullptr);
    }

    virtual Eigen::Matrix4f toMatrix() const = 0;

protected:
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) noexcept;
    Camera& operator=(Camera&&) noexcept;

private:
    Transform offset_;
    Eigen::Vector2f zrange_{0.1f, 100.0f};
    std::unique_ptr<Texture> hud_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAMERA_H
