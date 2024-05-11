//
//
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(QObject* parent)
    : QObject(parent) {
}

bool Renderer::init() {
    return true;
}

VOLCANO_GRAPHICS_END
