//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(GL3WGetProcAddressProc get_proc, int width, int height)
    : widht_(width)
    , height_(height) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0.0f, 0.0f, width, height);
}

Renderer::~Renderer() {
}

void Renderer::render(const View& view, Duration elapsed) {
    glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_GRAPHICS_END
