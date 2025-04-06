//
//
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer() {
}

Renderer::~Renderer() {
}

void Renderer::render(const View& view, Duration elapsed) {
    gl3wProcs = &gl3w_;
}

VOLCANO_GRAPHICS_END
