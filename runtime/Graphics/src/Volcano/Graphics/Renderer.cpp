//
//
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer()
    : widht_(0)
    , height_(0) {
}

Renderer::~Renderer() {
}

bool Renderer::init(int width, int height) {
    VOLCANO_ASSERT(SDL_GL_GetCurrentContext() != nullptr);

    auto ret = gl3wInit(&gl3w_, GL3WGetProcAddressProc(SDL_GL_GetProcAddress));
    if (ret != GL3W_OK) {
        return false;
    }

    widht_ = width;
    height_ = height;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0.0f, 0.0f, width, height);

    return true;
}

bool Renderer::begin() {
    return true;
}

void Renderer::end() {
}

void Renderer::update(Duration elapsed) {
    gl3wProcs = &gl3w_;

    glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_GRAPHICS_END
