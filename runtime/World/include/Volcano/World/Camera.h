//
//
#ifndef VOLCANO_WORLD_CAMERA_H
#define VOLCANO_WORLD_CAMERA_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Camera {
	float znear;
	float zfar;
	float ratio;
	float fov;

	Eigen::Matrix4f matrix() const noexcept {

	}


};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAMERA_H
