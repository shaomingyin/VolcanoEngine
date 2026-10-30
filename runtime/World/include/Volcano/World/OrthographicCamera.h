#ifndef VOLCANO_WORLD_ORTHOGRAPHICCAMERA_H
#define VOLCANO_WORLD_ORTHOGRAPHICCAMERA_H

#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

class OrthographicCamera: public Camera {
public:
    OrthographicCamera() noexcept;
    OrthographicCamera(float left, float right, float bottom, float top, float znear, float zfar);

    constexpr const Eigen::Vector4f& rect() const noexcept {
        return rect_;
    }

    void setRect(float left, float right, float bottom, float top);
    Eigen::Matrix4f toMatrix() const override;

private:
    Eigen::Vector4f rect_{0.0f, 1.0f, 0.0f, 1.0f};
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ORTHOGRAPHICCAMERA_H
