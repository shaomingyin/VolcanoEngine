//
//
#ifndef VOLCANO_ACOUSTICS_SPACE_H
#define VOLCANO_ACOUSTICS_SPACE_H

#include <Volcano/Acoustics/Common.h>

VOLCANO_ACOUSTICS_BEGIN

class Space {
public:
    Space(entt::registry& registry);
    virtual ~Space() = default;

public:
    void frame(Duration elapsed);

private:
    entt::registry& registry_;
};

VOLCANO_ACOUSTICS_END

#endif // VOLCANO_ACOUSTICS_SPACE_H
