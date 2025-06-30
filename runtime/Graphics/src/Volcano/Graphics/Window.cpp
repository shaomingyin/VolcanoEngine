//
//
#include <Volcano/Graphics/Window.h>

VOLCANO_GRAPHICS_BEGIN

Window::Window(SDL_Window* handle)
    : handle_(handle)
    , id_(GetWindowId(handle))
    , title_(GetWindowTitle(handle))
    , width_(GetWindowWidth(handle))
    , height_(GetWindowHeight(handle)) {
    if (handle_ == nullptr) {
        throw std::runtime_error("Invalid window handle.");
    }
}

Window::~Window() {
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

Uint32 Window::GetWindowId(SDL_Window* handle) {
    if (handle == nullptr) {
        throw std::invalid_argument("Invalid window handle.");
    }
    return SDL_GetWindowID(handle);
}

std::string Window::GetWindowTitle(SDL_Window* handle) {
    if (handle == nullptr) {
        throw std::invalid_argument("Invalid window handle.");
    }
    return std::string(SDL_GetWindowTitle(handle));
}

int Window::GetWindowWidth(SDL_Window* handle) {
    if (handle == nullptr) {
        throw std::invalid_argument("Invalid window handle.");
    }
    int width;
    SDL_GetWindowSize(handle, &width, NULL);
    return width;
}

int Window::GetWindowHeight(SDL_Window* handle) {
    if (handle == nullptr) {
        throw std::invalid_argument("Invalid window handle.");
    }
    int height;
    SDL_GetWindowSize(handle, NULL, &height);
    return height;
}

VOLCANO_GRAPHICS_END
