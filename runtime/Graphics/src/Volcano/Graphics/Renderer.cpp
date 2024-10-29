//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer()
    : widht_(0)
    , height_(0) {
}

Renderer::~Renderer() {
}

void Renderer::init(GL3WGetProcAddressProc get_proc, int width, int height) {
    auto ret = gl3wInit(&gl3w_, get_proc);
    if (ret != GL3W_OK) {
        throw Error(Errc::InvalidContext);
    }

    widht_ = width;
    height_ = height;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0.0f, 0.0f, width, height);
}

void Renderer::render(const View& view, Duration elapsed) {
    gl3wProcs = &gl3w_;

    glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_GRAPHICS_END
