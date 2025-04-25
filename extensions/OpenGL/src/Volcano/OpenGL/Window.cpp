//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/OpenGL/Window.h>

VOLCANO_OPENGL_BEGIN

Window::Window(const std::string& title, int width, int height)
    : Graphics::Window(SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL))
    , gl_(SDL_GL_CreateContext(handle())) {
    if (gl_ == nullptr) {
        throw std::runtime_error("Failed to create OpenGL context.");
    }
    auto gl_guard = scopeGuard([this] {
        SDL_GL_DestroyContext(gl_);
    });
    if (SDL_GL_MakeCurrent(handle(), gl_) != 0) {
        throw std::runtime_error("Failed to make current OpenGL context.");
    }
    if (gl3wInit(&gl3w_, GL3WGetProcAddressProc(SDL_GL_GetProcAddress)) != GL3W_OK) {
        throw std::runtime_error("Failed to initialize GL3W.");
    }
    gl_guard.dismiss();
}

Window::~Window() {
    SDL_GL_DestroyContext(gl_);
}

VOLCANO_OPENGL_END
