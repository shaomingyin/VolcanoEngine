//
//
#ifndef VOLCANO_WORLD_LOADER_H
#define VOLCANO_WORLD_LOADER_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

class Loader {
public:
    Loader(Scene& scene);
    virtual ~Loader() = default;

public:

private:
    Scene& scene_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_LOADER_H
