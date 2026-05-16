//
//
#ifndef VOLCANO_WORLD_CAMERA_H
#define VOLCANO_WORLD_CAMERA_H

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Camera {
    Transform offset;
    float znear{ 1.0f };
    float zfar{ 1000.0f };
};

struct PerspectiveCamera: Camera {
    float fov{ 60.0f };
	float aspect_ratio{ 1.0f };

    void set(float fov, float znear, float zfar, float aspect_ratio) noexcept {
        this->fov = fov;
        this->znear = znear;
        this->zfar = zfar;
		this->aspect_ratio = aspect_ratio;
    }

    Eigen::Matrix4f toMatrix() const noexcept {
        Eigen::Matrix4f m = Eigen::Matrix4f::Zero();
        float f = 1.0f / std::tan(fov * 0.5f);
        m(0, 0) = f / aspect_ratio;
        m(1, 1) = f;
        m(2, 2) = -(zfar + znear) / (zfar - znear);
        m(2, 3) = -(2.0f * zfar * znear) / (zfar - znear);
        m(3, 2) = -1.0f;
        return m;
    }
};

struct OrthographicCamera: public Camera {
    Eigen::Vector4f rect{0.0f, 1.0f, 0.0f, 1.0f};

    void set(float left, float right, float bottom, float top, float znear, float zfar) noexcept {
        rect = Eigen::Vector4f(left, right, bottom, top);
        this->znear = znear;
        this->zfar = zfar;
	}

    Eigen::Matrix4f toMatrix() const noexcept {
        Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
        m(0, 0) = 2.0f / (rect[1] - rect[0]);
        m(1, 1) = 2.0f / (rect[3] - rect[2]);
        m(2, 2) = -2.0f / (zfar - znear);
        m(0, 3) = -(rect[1] + rect[0]) / (rect[1] - rect[0]);
        m(1, 3) = -(rect[3] + rect[2]) / (rect[3] - rect[2]);
        m(2, 3) = -(zfar + znear) / (zfar - znear);
        return m;
    }
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAMERA_H
