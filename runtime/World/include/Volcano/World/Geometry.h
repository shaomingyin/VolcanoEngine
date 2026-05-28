//
//
#ifndef VOLCANO_WORLD_COLLISION_H
#define VOLCANO_WORLD_COLLISION_H

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Geometry {
    Eigen::Vector3f color;
};

struct Box: public Geometry {
    Eigen::Vector3f half_extents{ 1.0f, 1.0f, 1.0f };
};

struct Sphere : public Geometry {
    float radius{ 0.0f };
};

struct Capsule : public Geometry {
    float height{ 0.0f };
};

struct Cylinder : public Geometry {
    float height{ 0.0f };
};

struct Plane : public Geometry {
    Eigen::Vector3f normal{ 0.0f, 1.0f, 0.0f };
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COLLISION_H
