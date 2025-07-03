//
//
#include <SDL3/SDL_video.h>

#include <Volcano/Graphics/Renderer.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

VOLCANO_GRAPHICS_BEGIN

struct ScopedGL3W final {
    GL3WProcs& gl3w_;
    ScopedGL3W(GL3WProcs& gl3w): gl3w_(gl3w) { gl3wProcs = &gl3w_; }
    ~ScopedGL3W() { gl3wProcs = nullptr; }
};

Renderer::Renderer() {
    int ret = gl3wInit(&gl3w_, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress);
    if (ret != GL3W_OK) {
        throw std::runtime_error("Failed to init OpenGL procs.");
    }
    ScopedGL3W sg(gl3w_);
    nvg_ = nvgCreateGL3(0);
    if (nvg_ = nullptr) {
        throw std::runtime_error("Failed to create nvg context.");
    }
}

Renderer::~Renderer() {
    ScopedGL3W sg(gl3w_);
    nvgDeleteGL3(nvg_);
}

void Renderer::render(const View& view) {
    ScopedGL3W sg(gl3w_);
    // TODO
}

VOLCANO_GRAPHICS_END
