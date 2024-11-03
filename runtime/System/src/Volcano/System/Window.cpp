//
//
#include <GL/gl3w.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

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

    imgui_ = ImGui::CreateContext();
    if (imgui_ == nullptr) {
        throw Error(Errc::OutOfResource);
    }

    ImGui::SetCurrentContext(imgui_);

    if (!ImGui_ImplSDL2_InitForOpenGL(handle_, gl_context_)) {
        throw Error(Errc::OutOfResource);
    }

    auto imgui_sdl2_guard = scopeGuard([] {
        ImGui_ImplSDL2_Shutdown();
    });

    if (!ImGui_ImplOpenGL3_Init("#version 130")) {
        throw Error(Errc::OutOfResource);
    }

    auto imgui_gl3_guard = scopeGuard([] {
        ImGui_ImplOpenGL3_Shutdown();
    });

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    ImGui::StyleColorsDark();

    window_guard.dismiss();
    gl_context_guard.dismiss();
    imgui_sdl2_guard.dismiss();
    imgui_gl3_guard.dismiss();
}

Window::~Window() {
    if (flags_ & FlagDrawing) {
        endFrame();
    }

    gl3wProcs = &gl3w_;
    ImGui::SetCurrentContext(imgui_);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(imgui_);
    gl3wProcs = nullptr;

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

    ImGui_ImplSDL2_ProcessEvent(&evt);
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

    ImGui::SetCurrentContext(imgui_);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    flags_ |= FlagDrawing;

    return true;
}

void Window::endFrame() {
    if (flags_ & FlagDrawing) {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(handle_);
        gl3wProcs = nullptr;
        flags_ &= ~FlagDrawing;
    }
}

void Window::resize(int width, int height) {
    SDL_SetWindowSize(handle_, width, height);
}

VOLCANO_SYSTEM_END
