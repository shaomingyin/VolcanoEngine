//
//
#ifndef VOLCANO_WORLD_LIGHT_H
#define VOLCANO_WORLD_LIGHT_H

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Light {
    Eigen::Vector3f color{ 0.0f, 0.0f, 0.0f };
    float strength{ 0.0f };
};

struct DirectionalLight: Light {
    Eigen::Vector3f direction{ 0.0f, 0.0f, -1.0f };
};

struct PointLight: Light {
};

struct SpotLight: DirectionalLight {
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_LIGHT_H
