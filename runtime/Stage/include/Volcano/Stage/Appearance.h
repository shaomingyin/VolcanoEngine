//
//
#ifndef VOLCANO_STAGE_APPERANCE_H
#define VOLCANO_STAGE_APPERANCE_H

#include <string>
#include <vector>

#include <Volcano/Math.h>
#include <Volcano/Stage/Common.h>

VOLCANO_STAGE_BEGIN

struct Apperance {
    struct Model {
		std::string filepath;
        Transform offset;
    };

    std::vector<Model> models;
};

VOLCANO_STAGE_END

#endif // VOLCANO_STAGE_APPERANCE_H
