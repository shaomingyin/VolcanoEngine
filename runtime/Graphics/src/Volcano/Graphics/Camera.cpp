//
//
#include <Volcano/Graphics/Camera.h>

VOLCANO_GRAPHICS_BEGIN

Camera::Camera()
	: view_(Eigen::Affine3f::Identity())
	, projection_(Eigen::Matrix4f::Zero()) {
}

Camera::~Camera() {
}

void Camera::resetWorldTransform() {
	if (world_ != nullptr) {
		auto offset = view_;
		view_ = (*world) * offset;
		world_ = nullptr;
	}
}

void Camera::attachWorldTransform(Eigen::Affine3f& v) {
	resetWorldTransform();
	world_ = &v;
	auto offset = view_;
	view_ = v.inverse() * offset;
}

VOLCANO_GRAPHICS_END
