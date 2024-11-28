//
//
#ifndef VOLCANO_ACOUSTICS_SPACE_H
#define VOLCANO_ACOUSTICS_SPACE_H

#include <Volcano/World/Scene.h>
#include <Volcano/Acoustics/Common.h>

VOLCANO_ACOUSTICS_BEGIN

class Space {
public:
    Space(World::Scene& scene);
    virtual ~Space() = default;

public:
    virtual void update(Duration elapsed);
    virtual void render();

private:
    World::Scene& scene_;
};

VOLCANO_ACOUSTICS_END

#endif // VOLCANO_ACOUSTICS_SPACE_H
