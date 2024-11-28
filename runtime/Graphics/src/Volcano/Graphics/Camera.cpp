//
//
#include <Volcano/Graphics/Camera.h>

VOLCANO_GRAPHICS_BEGIN

Camera::Camera()
	: projection_(Eigen::Matrix4f::Zero()) {
}

Camera::~Camera() {
}

VOLCANO_GRAPHICS_END
