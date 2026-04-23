//
//
#ifndef VOLCANO_WORLD_COLLISION_H
#define VOLCANO_WORLD_COLLISION_H

#include <vector>

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Collision {
    struct Shape {
        Transform offset;
    };

    struct BoxShape: Shape {
        Eigen::Vector3f half_size;
    };

    float mass;
    std::vector<BoxShape> box_shapes;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COLLISION_H
