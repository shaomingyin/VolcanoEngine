//
//
#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Window.h>

VOLCANO_SYSTEM_BEGIN

Window::Window(const std::string& title, int width, int height)
	: flags_(0)
    , handle_(nullptr)
    , id_(0)
    , gl_context_(nullptr) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    handle_ = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
        SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (handle_ == nullptr) {
        throw Error(Errc::OutOfResource);
    }

    auto window_guard = scopeGuard([this] {
        SDL_DestroyWindow(handle_);
        handle_ = nullptr;
    });

    gl_context_ = SDL_GL_CreateContext(handle_);
    if (gl_context_ == nullptr) {
        throw Error(Errc::OutOfResource);
    }

    auto gl_context_guard = scopeGuard([this] {
        SDL_GL_DeleteContext(gl_context_);
        gl_context_ = nullptr;
    });

    SDL_ShowWindow(handle_);
    flags_ = FlagVisible;
    id_ = SDL_GetWindowID(handle_);

    if (SDL_GL_MakeCurrent(handle_, gl_context_) != 0) {
        throw Error(Errc::InvalidContext);
    }

    auto ret = gl3wInit(&gl3w_, GL3WGetProcAddressProc(SDL_GL_GetProcAddress));
    if (ret != GL3W_OK) {
        throw Error(Errc::InvalidContext);
    }

    window_guard.dismiss();
    gl_context_guard.dismiss();
}

Window::~Window() {
    if (gl_context_ != nullptr) {
        SDL_GL_DeleteContext(gl_context_);
    }
    if (handle_ != nullptr) {
        SDL_DestroyWindow(handle_);
    }
}

void Window::handleEvent(const SDL_Event& evt) {
    if (evt.type == SDL_WINDOWEVENT || evt.window.windowID == id_) {
        switch (evt.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            flags_ |= FlagVisible;
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            flags_ &= ~FlagVisible;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            flags_ &= ~FlagVisible;
            break;
        default:
            break;
        }
    }
}

VOLCANO_SYSTEM_END
