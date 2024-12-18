//
//
#ifndef VOLCANO_GRAPHICS_CAMERA_H
#define VOLCANO_GRAPHICS_CAMERA_H

#include <Volcano/World/Transformable.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Camera: public World::Transformable {
public:
	Camera();
	virtual ~Camera();

public:
#if 0
	Eigen::Affine3f& view() {
		return view_;
	}

	const Eigen::Affine3f& view() const {
		return view_;
	}

	Eigen::Matrix4f& viewMatrix() {
		return view_.matrix();
	}

	const Eigen::Matrix4f& viewMatrix() const {
		return view_.matrix();
	}
#endif

	Eigen::Projective3f& projection() {
		return projection_;
	}

	const Eigen::Projective3f& projection() const {
		return projection_;
	}

	Eigen::Matrix4f& projectionMatrix() {
		return projection_.matrix();
	}

	const Eigen::Matrix4f& projectionMatrix() const {
		return projection_.matrix();
	}

	void lookTo(Eigen::Vector3f pos, Eigen::Vector3f direction, Eigen::Vector3f up) {
#if 0
		auto eye = worldTransform() * pos;
		auto f = direction.normalized();
		auto u = up.normalized();
		auto s = f.cross(u).normalized();
		u = s.cross(f);
		projection_ = Eigen::Matrix4f::Identity();
		projection_(0, 0) = s.x();
		projection_(0, 1) = s.y();
		projection_(0, 2) = s.z();
		projection_(1, 0) = u.x();
		projection_(1, 1) = u.y();
		projection_(1, 2) = u.z();
		projection_(2, 0) = -f.x();
		projection_(2, 1) = -f.y();
		projection_(2, 2) = -f.z();
		projection_(0, 3) = -s.dot(eye);
		projection_(1, 3) = -u.dot(eye);
		projection_(2, 3) = f.dot(eye);
#endif
	}

	void lookAt(Eigen::Vector3f pos, Eigen::Vector3f center, Eigen::Vector3f up) {
		lookTo(pos, center - pos, up);
	}

	void ortho(float left, float right, float top, float bottom, float zNear, float zFar) {
#if 0
		auto& view = Eigen::Affine3f::Identity();
		view(0, 0) = 2.0f / (right - left);
		view(1, 1) = 2.0f / (top - bottom);
		view(2, 2) = -2.0f / (zFar - zNear);
		view(0, 3) = -(right + left) / (right - left);
		view(1, 3) = -(top + bottom) / (top - bottom);
		view(2, 3) = -(zFar + zNear) / (zFar - zNear);
		localTransform() = view;
#endif
	}

	void perspective(float fov, float aspect, float zNear, float zFar) {
#if 0
		view_ = Eigen::Matrix4f::Zero();
		float tanHalfFov = tan(fov / 2.0f);
		view_(0, 0) = 1.0f / (aspect * tanHalfFov);
		view_(1, 1) = 1.0f / tanHalfFov;
		view_(2, 2) = -(zFar + zNear) / (zFar - zNear);
		view_(2, 3) = -1.0f;
		view_(3, 2) = -(2.0f * zFar * zNear) / (zFar - zNear);
#endif
	}

private:
	Eigen::Projective3f projection_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERA_H
