//
//
#ifndef VOLCANO_WORLD_APPERANCE_H
#define VOLCANO_WORLD_APPERANCE_H

#include <string>
#include <vector>

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Apperance {
    struct Model {
		std::string filepath;
        Transform offset;
    };

    std::vector<Model> models;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_APPERANCE_H
