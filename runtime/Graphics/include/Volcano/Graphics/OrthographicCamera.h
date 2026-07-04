//
//
#ifndef VOLCANO_GRAPHICS_ORTHOGRAPHICSCAMERA_H
#define VOLCANO_GRAPHICS_ORTHOGRAPHICSCAMERA_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Camera.h>

VOLCANO_GRAPHICS_BEGIN

class OrthographicCamera: public Camera {
public:
    OrthographicCamera() noexcept;
    OrthographicCamera(const OrthographicCamera&) noexcept = default;
    OrthographicCamera(OrthographicCamera&&) noexcept = default;
    OrthographicCamera(float left, float right, float bottom, float top, float znear, float zfar) noexcept;

public:
    OrthographicCamera& operator=(const OrthographicCamera&) noexcept = default;
    OrthographicCamera& operator=(OrthographicCamera&&) noexcept = default;

    Eigen::Matrix4f toMatrix() const noexcept override;

    friend void to_json(nlohmann::json& json, const OrthographicCamera& v);
    friend void from_json(const nlohmann::json& json, OrthographicCamera& v);

private:
    Eigen::Vector4f rect_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_ORTHOGRAPHICSCAMERA_H
