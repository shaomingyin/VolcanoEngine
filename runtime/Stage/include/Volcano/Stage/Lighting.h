//
//
#ifndef VOLCANO_STAGE_LIGHT_H
#define VOLCANO_STAGE_LIGHT_H

#include <Volcano/Math.h>
#include <Volcano/Stage/Common.h>

VOLCANO_STAGE_BEGIN

struct Light {
    Transform offset;
    Eigen::Vector3f color{ 0.0f, 0.0f, 0.0f };
    float strength{ 0.0f };
};

struct DirectionalLight: Light {
    Eigen::Vector3f direction{ 0.0f, 0.0f, -1.0f };
};

struct PointLight: Light {
    float range{ 1.0f };
};

struct SpotLight: PointLight {
    float inner_angle{ 20.0f };
	float outer_angle{ 30.0f };
};

VOLCANO_STAGE_END

#endif // VOLCANO_STAGE_LIGHT_H
