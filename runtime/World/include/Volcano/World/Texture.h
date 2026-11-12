#ifndef VOLCANO_WORLD_TEXTURE_H
#define VOLCANO_WORLD_TEXTURE_H

#include <Volcano/Size.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Texture {
public:
    virtual ~Texture() = default;

    virtual Size size() const noexcept = 0;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TEXTURE_H
