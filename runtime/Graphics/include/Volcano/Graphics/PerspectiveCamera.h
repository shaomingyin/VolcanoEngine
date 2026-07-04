//
//
#ifndef VOLCANO_GRAPHICS_PERSPECTIVECAMERA_H
#define VOLCANO_GRAPHICS_PERSPECTIVECAMERA_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Camera.h>

VOLCANO_GRAPHICS_BEGIN

class PerspectiveCamera: public Camera {
public:
    PerspectiveCamera() noexcept;
    PerspectiveCamera(float fov, float aspect, float znear, float zfar) noexcept;
    PerspectiveCamera(const PerspectiveCamera&) noexcept = default;
    PerspectiveCamera(PerspectiveCamera&&) noexcept = default;

public:
    PerspectiveCamera& operator=(const PerspectiveCamera&) noexcept = default;
    PerspectiveCamera& operator=(PerspectiveCamera&&) noexcept = default;

    Eigen::Matrix4f toMatrix() const noexcept override;

    friend void to_json(nlohmann::json& json, const PerspectiveCamera& v);
    friend void from_json(const nlohmann::json& json, PerspectiveCamera& v);

private:
    float fov_;
	float aspect_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PERSPECTIVECAMERA_H
