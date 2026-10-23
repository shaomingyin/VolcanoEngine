#ifndef VOLCANO_GRAPHICS_GEOMETRYPASS_H
#define VOLCANO_GRAPHICS_GEOMETRYPASS_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

struct RenderContext;

class GeometryPass final {
public:
    void render(const RenderContext& context);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_GEOMETRYPASS_H
