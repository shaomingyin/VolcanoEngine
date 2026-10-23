#ifndef VOLCANO_GRAPHICS_MATERIAL_H
#define VOLCANO_GRAPHICS_MATERIAL_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Math/Color.h>

VOLCANO_GRAPHICS_BEGIN

class Texture;

struct Material final {
    Math::Colorf color = Math::Colorf::White;
    const Texture* texture = nullptr;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MATERIAL_H
