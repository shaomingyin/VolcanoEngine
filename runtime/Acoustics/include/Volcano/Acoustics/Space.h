//
//
#ifndef VOLCANO_ACOUSTICS_SPACE_H
#define VOLCANO_ACOUSTICS_SPACE_H

#include <Volcano/Acoustics/Common.h>

VOLCANO_ACOUSTICS_BEGIN

class Space {
public:
    Space();

public:
    virtual void update();
    virtual void render();
};

VOLCANO_ACOUSTICS_END

#endif // VOLCANO_ACOUSTICS_SPACE_H
