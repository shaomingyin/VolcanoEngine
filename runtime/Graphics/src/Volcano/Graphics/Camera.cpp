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
		world_ = nullptr;
	}
}

void Camera::setWorldTransform(Eigen::Affine3f& v) {
	resetWorldTransform();
	world_ = &v;

}

VOLCANO_GRAPHICS_END
