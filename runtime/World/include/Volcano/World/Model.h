//
//
#ifndef VOLCANO_WORLD_MODEL_H
#define VOLCANO_WORLD_MODEL_H

#include <string>
#include <vector>
#include <filesystem>

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Mesh {
    uint32_t index;
    uint32_t count;
    std::filesystem::path material;
};

struct Model {
    Transform transform;
    std::filesystem::path filepath;
    std::vector<Mesh> meshes;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MODEL_H
