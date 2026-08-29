//
//
#ifndef VOLCANO_WORLD_PERSPECTIVECAMERA_H
#define VOLCANO_WORLD_PERSPECTIVECAMERA_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

class PerspectiveCamera: public Camera {
public:
    PerspectiveCamera() noexcept;
    PerspectiveCamera(float fov, float aspect, float znear, float zfar) noexcept;
    PerspectiveCamera(const PerspectiveCamera&) noexcept = default;
    PerspectiveCamera(PerspectiveCamera&&) noexcept = default;

public:
    PerspectiveCamera& operator=(const PerspectiveCamera&) noexcept = default;
    PerspectiveCamera& operator=(PerspectiveCamera&&) noexcept = default;

    float& fov() noexcept {
        return fov_;
    }

    const float& fov() const noexcept {
        return fov_;
    }

    float& aspect() noexcept {
        return aspect_;
    }

    const float& aspect() const noexcept {
        return aspect_;
    }

    Eigen::Matrix4f toMatrix() const noexcept override;

private:
    float fov_;
	float aspect_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_PERSPECTIVECAMERA_H
