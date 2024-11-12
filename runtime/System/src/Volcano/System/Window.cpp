//
//
#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Window.h>

VOLCANO_SYSTEM_BEGIN

Window::Window(const std::string& title, int width, int height, int flags)
	: Graphics::Target(width, height)
    , flags_(flags & (FlagResizable | FlagClear))
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

    Uint32 window_flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    if (flags_ & FlagResizable) {
        window_flags |= SDL_WINDOW_RESIZABLE;
    }

    handle_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
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
    id_ = SDL_GetWindowID(handle_);

    if (SDL_GL_MakeCurrent(handle_, gl_context_) != 0) {
        throw Error(Errc::InvalidContext);
    }

    auto ret = gl3wInit(&gl3w_, GL3WGetProcAddressProc(SDL_GL_GetProcAddress));
    if (ret != GL3W_OK) {
        throw Error(Errc::InvalidContext);
    }

    gl3wProcs = &gl3w_;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    window_guard.dismiss();
    gl_context_guard.dismiss();
}

Window::~Window() {
    if (flags_ & FlagDrawing) {
        endFrame();
    }

    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(handle_);
}

void Window::handleEvent(const SDL_Event& evt) {
    if (evt.type != SDL_WINDOWEVENT && evt.window.windowID != id_) {
        return;
    }

    switch (evt.window.event) {
    case SDL_WINDOWEVENT_SHOWN:
        flags_ |= FlagVisible;
        break;
    case SDL_WINDOWEVENT_HIDDEN:
        flags_ &= ~FlagVisible;
        break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        Graphics::Target::resize(evt.window.data1, evt.window.data2);
        break;
    default:
        break;
    }
}

bool Window::beginFrame() {
    if (flags_ & FlagDrawing) {
        SDL_GL_SwapWindow(handle_);
    }

    if (!(flags_ & FlagVisible)) {
        return false;
    }
    
    if (SDL_GL_MakeCurrent(handle_, gl_context_) != 0) {
        return false;
    }

    gl3wProcs = &gl3w_;
    glViewport(0.0f, 0.0f, width_, height_);

    flags_ |= FlagDrawing;

    return true;
}

void Window::endFrame() {
    if (flags_ & FlagDrawing) {
        SDL_GL_SwapWindow(handle_);
        gl3wProcs = nullptr;
        flags_ &= ~FlagDrawing;
    }
}

void Window::resize(int width, int height) {
    SDL_SetWindowSize(handle_, width, height);
}

VOLCANO_SYSTEM_END
