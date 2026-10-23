#ifndef VOLCANO_GRAPHICS_RENDERCONTEXT_H
#define VOLCANO_GRAPHICS_RENDERCONTEXT_H

#include <span>
#include <Volcano/Graphics/Material.h>
#include <Volcano/Math/Affine.h>
#include <Volcano/Math/Matrix.h>

VOLCANO_GRAPHICS_BEGIN

class Mesh;
class Target;

struct DrawItem final {
    const Mesh& mesh;
    Material material{};
    Math::Affine3f transform{};
    bool transparent = false;
};

// Non-owning frame inputs; resources must remain alive throughout Renderer::render.
struct RenderContext final {
    Target& target;
    std::span<const DrawItem> items{};
    Math::Matrix4f projection{};
    Math::Matrix4f view{};
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERCONTEXT_H
