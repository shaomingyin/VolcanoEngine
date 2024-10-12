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

bool Renderer::init(GL3WGetProcAddressProc gl_address_proc, int width, int height) {
    VOLCANO_ASSERT(gl_address_proc != nullptr);

    auto ret = gl3wInit(&gl3w_, gl_address_proc);
    if (ret != GL3W_OK) {
        return false;
    }

    widht_ = width;
    height_ = height;

    return true;
}

bool Renderer::begin() {
    return true;
}

void Renderer::end() {
}

void Renderer::update(std::chrono::steady_clock::duration elapsed) {
    gl3wProcs = &gl3w_;
}

VOLCANO_GRAPHICS_END
