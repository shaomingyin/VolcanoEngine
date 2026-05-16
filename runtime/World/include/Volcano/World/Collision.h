//
//
#ifndef VOLCANO_WORLD_COLLISION_H
#define VOLCANO_WORLD_COLLISION_H

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct CollisionShape {
    Transform offset;
};

struct BoxCollisionShape: CollisionShape {
    Eigen::Vector3f half_extents{ 0.0f, 0.0f, 0.0f };
};

struct SphereCollisionShape: CollisionShape {
    float radius{ 0.0f };
};

struct CapsuleCollisionShape: SphereCollisionShape {
    float height{ 0.0f };
};

struct CylinderCollisionShape: SphereCollisionShape {
    float height{ 0.0f };
};

struct PlaneCollisionShape: CollisionShape {
    Eigen::Vector3f normal{ 0.0f, 1.0f, 0.0f };
};

struct MeshCollisionShape: CollisionShape {
    // TODO
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COLLISION_H
