//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Frontend.h>

VOLCANO_SYSTEM_BEGIN

Frontend::Frontend(ResourceManager& resource_manager)
    : resource_manager_(resource_manager)
    , game_world_(nullptr)
    , flags_(0)
    , window_(nullptr)
    , window_id_(0)
    , gl_context_(nullptr)
    , current_view_(0) {
}

Frontend::~Frontend() {
    if (window_ != nullptr) {
        if (gl_context_ != nullptr) {
            SDL_GL_DeleteContext(gl_context_);
        }
        SDL_DestroyWindow(window_);
    }
}

bool Frontend::init(const std::string& title, int width, int height) {
    VOLCANO_ASSERT(window_ == nullptr);
    VOLCANO_ASSERT(gl_context_ == nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    window_ = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window_ == nullptr) {
        return false;
    }

    auto window_guard = scopeGuard([this] {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    });

    gl_context_ = SDL_GL_CreateContext(window_);
    if (gl_context_ == nullptr) {
        return false;
    }

    auto gl_context_guard = scopeGuard([this] {
        SDL_GL_DeleteContext(gl_context_);
        gl_context_ = nullptr;
    });

    window_id_ = SDL_GetWindowID(window_);
    SDL_ShowWindow(window_);
    SDL_GL_MakeCurrent(window_, gl_context_);

    if (!graphics_renderer_.init(width, height)) {
        return false;
    }

    flags_ = FlagWindowVisible;

    gl_context_guard.dismiss();
    window_guard.dismiss();

    return true;
}

void Frontend::bind(Game::World* game_world) {
    game_world_ = game_world;
}

void Frontend::handleEvent(const SDL_Event& evt) {
    VOLCANO_ASSERT(window_ != nullptr);

    if (evt.type != SDL_WINDOWEVENT || evt.window.windowID != window_id_) {
        return;
    }

    switch (evt.window.event) {
    case SDL_WINDOWEVENT_SHOWN:
        flags_ |= FlagWindowVisible;
        break;
    case SDL_WINDOWEVENT_HIDDEN:
        flags_ &= ~FlagWindowVisible;
        break;
    case SDL_WINDOWEVENT_CLOSE:
        flags_ |= FlagQuit;
        break;
    default:
        break;
    }
}

void Frontend::frame(Duration elapsed) {
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

void Frontend::buildGraphicsView() {
    if (VOLCANO_UNLIKELY(game_world_ == nullptr)) {
        return;
    }

    //int exp = current_view_;
    //while (current_view_.compare_exchange_strong(exp, !current_view_));
}

VOLCANO_SYSTEM_END
