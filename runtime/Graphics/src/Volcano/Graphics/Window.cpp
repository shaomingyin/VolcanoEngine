//
//
#include <Volcano/Graphics/Window.h>

VOLCANO_GRAPHICS_BEGIN

static int GetWindowWidth(SDL_Window* handle) {
    int width;
    SDL_GetWindowSize(handle, &width, NULL);
    return width;
}

static int GetWindowHeight(SDL_Window* handle) {
    int height;
    SDL_GetWindowSize(handle, NULL, &height);
    return height;
}

Window::Window(SDL_Window* handle)
    : Target(GetWindowWidth(handle), GetWindowHeight(handle))
    , handle_(handle)
    , id_(SDL_GetWindowID(handle)) {
    if (handle_ == nullptr) {
        throw std::runtime_error("Invalid window handle.");
    }
}

Window::~Window() {
    SDL_DestroyWindow(handle_);
}

void Window::handleEvent(const SDL_Event& evt) {
    if (evt.window.windowID != id_) {
        return;
    }
    switch (evt.window.type) {
    case SDL_EVENT_WINDOW_RESIZED:
        Target::resize(evt.window.data1, evt.window.data2);
        break;
    case SDL_EVENT_WINDOW_SHOWN:
        visible_ = true;
        break;
    case SDL_EVENT_WINDOW_HIDDEN:
        visible_ = false;
        break;
    }
}

void Window::resize(int width, int height) {
    SDL_SetWindowSize(handle_, width, height);
}

VOLCANO_GRAPHICS_END
