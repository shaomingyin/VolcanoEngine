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

bool Renderer::beginFrame() {
    if (View::beginFrame()) {
        return true;
    }
    return false;
}

void Renderer::commitFrame() {
    View::commitFrame();
}

void Renderer::cancelFrame() {
    View::cancelFrame();
}

VOLCANO_GRAPHICS_END
