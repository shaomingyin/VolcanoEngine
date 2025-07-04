//
//
#include <SDL3/SDL_timer.h>

#include <Volcano/Framework/Base.h>

VOLCANO_FRAMEWORK_BEGIN

Base::Base(SDL_Storage* rootfs, Game& game)
    : Context(rootfs, SDL_OpenUserStorage("Volcano", game.name().c_str(), 0))
    , game_(game) {
}

SDL_AppResult Base::update() noexcept {
    runAllTasks();
    auto curr = Clock::now();
    frame(curr - frame_last_);
    frame_last_ = curr;
    if (isQuit()) {
        if (state() == State::Error) {
            return SDL_APP_FAILURE;
        }
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

void Base::frame(Duration elapsed) {
    switch (state()) {
    case State::Playing:
        physics_world_.update(elapsed);
        break;
    case State::Loading:
        break;
    default:
        break;
    }
}

void Base::loadConfig(const nlohmann::json& j) {
    Context::loadConfig(j);
}

void Base::loadScene(const nlohmann::json& j) {
    Context::loadScene(j);
}

VOLCANO_FRAMEWORK_END
