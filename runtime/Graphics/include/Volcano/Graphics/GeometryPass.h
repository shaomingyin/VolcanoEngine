#ifndef VOLCANO_GRAPHICS_GEOMETRYPASS_H
#define VOLCANO_GRAPHICS_GEOMETRYPASS_H

#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Pass.h>

VOLCANO_GRAPHICS_BEGIN

class GeometryPass final : public Pass {
public:
    GeometryPass();

    void render(const View& view);

private:
    ObjectUniforms uniforms_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_GEOMETRYPASS_H
