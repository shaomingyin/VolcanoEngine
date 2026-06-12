//
//
#ifndef VOLCANO_WORLD_PHYSICAL_H
#define VOLCANO_WORLD_PHYSICAL_H

#include <variant>

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct BoxCollisionShape {
    float length = 1.0f;
    float width = 1.0f;
    float height = 1.0f;
};

struct SphereCollisionShape {
    float radius = 1.0f;
};

struct RigidBody {
    Transform offset;
    float mass = 1.0f;
    Eigen::Vector3f center_of_mass = { 0.0f, 0.0f, 0.0f };

    std::variant <
        BoxCollisionShape,
        SphereCollisionShape
    > shape;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_PHYSICAL_H
