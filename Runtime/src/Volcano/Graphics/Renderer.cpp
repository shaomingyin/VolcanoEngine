//
//
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(QObject* parent)
    : View(parent) {
}

bool Renderer::init() {
    return true;
}

void Renderer::beginFrame() {
}

void Renderer::endFrame() {
}

VOLCANO_GRAPHICS_END
