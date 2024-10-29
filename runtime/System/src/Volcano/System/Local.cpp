//
//
#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Local.h>

VOLCANO_SYSTEM_BEGIN

Local::Local(const std::filesystem::path& root, const std::filesystem::path& init)
    : Base(root, init)
    , flags_(0)
    , window_(nullptr)
    , window_id_(0)
    , gl_context_(nullptr)
    , current_view_(0) {
}

Local::~Local() {
    if (gl_context_ != nullptr) {
        SDL_GL_DeleteContext(gl_context_);
    }
    if (window_ != nullptr) {
        SDL_DestroyWindow(window_);
    }
}

void Local::init() {
    Base::init();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    auto title = settings().get<std::string>("/local/title", "Untitled");
    auto width = settings().get<int>("/local/width", 1024);
    auto height = settings().get<int>("/local/height", 768);
    auto color_depth = settings().get<int>("/local/color_depth", 32);

    window_ = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window_ == nullptr) {
        throw Error(Errc::OutOfResource);
    }

    auto window_guard = scopeGuard([this] {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    });

    gl_context_ = SDL_GL_CreateContext(window_);
    if (gl_context_ == nullptr) {
        throw Error(Errc::OutOfResource);
    }

    auto gl_context_guard = scopeGuard([this] {
        SDL_GL_DeleteContext(gl_context_);
        gl_context_ = nullptr;
    });

    window_id_ = SDL_GetWindowID(window_);
    SDL_ShowWindow(window_);
    SDL_GL_MakeCurrent(window_, gl_context_);

    graphics_renderer_.init(GL3WGetProcAddressProc(SDL_GL_GetProcAddress), width, height);
    flags_ = FlagWindowVisible;

    window_guard.dismiss();
    gl_context_guard.dismiss();
}

void Local::handleEvent(const SDL_Event& evt) {
    VOLCANO_ASSERT(window_ != nullptr);
    if (evt.type != SDL_WINDOWEVENT || evt.window.windowID != window_id_) {
        switch (evt.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            flags_ |= FlagWindowVisible;
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            flags_ &= ~FlagWindowVisible;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            stop();
            break;
        default:
            break;
        }
    }
}

void Local::frame(Duration elapsed) {
	Base::frame(elapsed);

    VOLCANO_ASSERT(window_ != nullptr);
    VOLCANO_ASSERT(gl_context_ != nullptr);

    buildGraphicsView();

    if (VOLCANO_LIKELY(flags_ & FlagWindowVisible) && (SDL_GL_MakeCurrent(window_, gl_context_) == 0)) {
        graphics_renderer_.render(views_[current_view_], elapsed);
        SDL_GL_SwapWindow(window_);
        //int exp = current_view_;
        //while (current_view_.compare_exchange_strong(exp, !current_view_));
    }
}

void Local::buildGraphicsView() {
    //int exp = current_view_;
    //while (current_view_.compare_exchange_strong(exp, !current_view_));
}

VOLCANO_SYSTEM_END
