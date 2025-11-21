//
//
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(Context& context, sf::RenderTarget& target)
    : context_(context)
    , target_(target) {
}

void Renderer::render(const View& view) {
}

VOLCANO_GRAPHICS_END
