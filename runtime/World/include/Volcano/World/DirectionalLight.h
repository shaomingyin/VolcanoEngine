//
//
#ifndef VOLCANO_GMAE_DIRECTIONALLIGHT_H
#define VOLCANO_GMAE_DIRECTIONALLIGHT_H

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

struct DirectionalLight: Light {
	Eigen::Vector3f direction_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_GMAE_DIRECTIONALLIGHT_H
