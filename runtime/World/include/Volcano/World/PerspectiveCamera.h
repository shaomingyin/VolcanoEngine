#ifndef VOLCANO_WORLD_PERSPECTIVECAMERA_H
#define VOLCANO_WORLD_PERSPECTIVECAMERA_H

#include <cmath>
#include <stdexcept>

#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

class PerspectiveCamera: public Camera {
public:
    PerspectiveCamera() noexcept;
    PerspectiveCamera(float fov, float aspect, float znear, float zfar);

    constexpr float fov() const noexcept {
        return fov_;
    }

    constexpr void setFov(float value) {
        if (!std::isfinite(value) || value <= 0.0f || value >= 180.0f) {
            throw std::invalid_argument("Perspective field of view must be finite and between 0 and 180 degrees");
        }

        fov_ = value;
    }

    constexpr float aspect() const noexcept {
        return aspect_;
    }

    constexpr void setAspect(float value) {
        if (!std::isfinite(value) || value <= 0.0f) {
            throw std::invalid_argument("Perspective aspect must be finite and positive");
        }

        aspect_ = value;
    }

    void setZRange(float znear, float zfar) override;
    Eigen::Matrix4f toMatrix() const override;

private:
    float fov_ = 90.0f;
    float aspect_ = 1.0f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_PERSPECTIVECAMERA_H
