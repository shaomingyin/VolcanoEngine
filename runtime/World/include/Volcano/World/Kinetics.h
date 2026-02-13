//
//
#ifndef VOLCANO_WORLD_KINETICS_H
#define VOLCANO_WORLD_KINETICS_H

#include <vector>
#include <variant>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transform.h>

VOLCANO_WORLD_BEGIN

struct Kinetics {
    struct Box {
        Eigen::Vector3f half_size;
    };

    struct Sphere {
        float radius;
    };

    struct Shape {
        std::variant<Box, Sphere> def;
        Transform offset;
    };

    std::vector<Shape> shape_set;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_KINETICS_H
