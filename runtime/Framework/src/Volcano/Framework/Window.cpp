//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/Framework/Window.h>

VOLCANO_FRAMEWORK_BEGIN

Window::Window(const std::string& title, int w, int h)
    : title_(title) {
    handle_ = SDL_CreateWindow(title_.c_str(), w, h, SDL_WINDOW_OPENGL);
    if (handle_ == nullptr) {
        throw std::runtime_error("Failed to create window.");
    }
    auto handle_guard = scopeGuard([this] {
        SDL_DestroyWindow(handle_);
    });

    gl_context_ = SDL_GL_CreateContext(handle_);
    if (gl_context_ == nullptr) {
        throw std::runtime_error("Failed to create OpenGL context.");
    }
    auto gl_context_guard = scopeGuard([this] {
        SDL_GL_DestroyContext(gl_context_);
    });

    flags_ = FlagVisible;
    id_ = SDL_GetWindowID(handle_);
    SDL_GetWindowSize(handle_, &width_, &height_);

    handle_guard.dismiss();
    gl_context_guard.dismiss();
}

Window::~Window() {
    SDL_GL_DestroyContext(gl_context_);
    SDL_DestroyWindow(handle_);
}

void Window::show() {
    SDL_ShowWindow(handle_);
}

void Window::hide() {
    SDL_HideWindow(handle_);
}

void Window::event(const SDL_Event& evt) {
    if (evt.window.windowID != id_) {
        return;
    }
    switch (evt.window.type) {
    case SDL_EVENT_WINDOW_RESIZED:
        width_ = evt.window.data1;
        height_ = evt.window.data2;
        break;
    case SDL_EVENT_WINDOW_SHOWN:
        flags_ |= FlagVisible;
        break;
    case SDL_EVENT_WINDOW_HIDDEN:
        flags_ &= ~FlagVisible;
        break;
    }
}

void Window::resize(int width, int height) {
    SDL_SetWindowSize(handle_, width, height);
}

VOLCANO_FRAMEWORK_END
