//
//
#include <Volcano/OpenGL/Window.h>
#include <Volcano/OpenGL/Renderer.h>

VOLCANO_OPENGL_BEGIN

Renderer::Renderer() {
}

Renderer::~Renderer() {
}

Graphics::Window* Renderer::createWindow(const std::string& title, int width, int height) {
    return new Window(title, width, height);
}

Graphics::Texture* Renderer::createTexture(int width, int height) {
    return nullptr;
}

VOLCANO_OPENGL_END
