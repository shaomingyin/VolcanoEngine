//
//
#ifndef VOLCANO_WORLD_VISUAL_H
#define VOLCANO_WORLD_VISUAL_H

#include <vector>
#include <variant>
#include <filesystem>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transform.h>

VOLCANO_WORLD_BEGIN

struct Visual {
    struct Box {
        Eigen::Vector3f half_size;
    };

    struct Sphere {
        float radius;
    };

    struct Mesh {
        int foo;
    };

    struct MeshResource {
        std::filesystem::path filepath;
    };

    struct Shape {
        std::variant<Box, Sphere, Mesh, MeshResource> def;
        Transform offset;
    };

    std::vector<Shape> shape_set;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_VISUAL_H
