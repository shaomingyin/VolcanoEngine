#ifndef VOLCANO_WORLD_MODEL_H
#define VOLCANO_WORLD_MODEL_H

#include <filesystem>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Model final {
    std::filesystem::path source;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MODEL_H
