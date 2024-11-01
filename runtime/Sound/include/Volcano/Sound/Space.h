//
//
#ifndef VOLCANO_SOUND_SPACE_H
#define VOLCANO_SOUND_SPACE_H

#include <Volcano/Sound/Common.h>

VOLCANO_SOUND_BEGIN

class Space {
public:
    Space() = default;
    virtual ~Space() = default;

public:
    void frame(Duration elapsed);
};

VOLCANO_SOUND_END

#endif // VOLCANO_SOUND_SPACE_H
