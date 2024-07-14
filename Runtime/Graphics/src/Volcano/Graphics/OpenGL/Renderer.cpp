//
//
#include <Volcano/Graphics/OpenGL/Renderer.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

Renderer::Renderer(QObject* parent)
    : Graphics::Renderer(parent) {
}

bool Renderer::doInit(int width, int height) {
    return true;
}

VOLCANO_GRAPHICS_OPENGL_END
