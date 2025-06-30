//
//
#ifndef VOLCANO_GRAPHICS_TEXTURE_H
#define VOLCANO_GRAPHICS_TEXTURE_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

class Texture: public Target {
public:
    Texture(int width, int height)
        : Target(width, height) {
    }
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TEXTURE_H
