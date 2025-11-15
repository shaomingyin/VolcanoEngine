//
//
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(int width, int height) {
    VOLCANO_ASSERT(gl3wProcs != nullptr);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Renderer::resize(int width, int height) {
    width_ = width;
    height_ = height;
}

void Renderer::beginFrame() {
    glViewport(0, 0, width_, height_);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::endFrame() {
}

VOLCANO_GRAPHICS_END
