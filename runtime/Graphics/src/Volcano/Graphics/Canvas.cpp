//
//
#include <Volcano/Graphics/Canvas.hpp>

VOLCANO_GRAPHICS_BEGIN

Canvas::Canvas(Context& context)
    : Renderable(context) {
}

void Canvas::render() {
    draw(render_texture_, sf::RenderStates::Default);
    // TODO render render_texture_
}

VOLCANO_GRAPHICS_END
