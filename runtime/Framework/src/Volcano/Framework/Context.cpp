//
//
#include <SDL3/SDL_timer.h>

#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

Context::Context(SDL_Storage* rootfs, SDL_Storage* userfs)
    : state_(State::Idle)
    , rootfs_(rootfs)
    , userfs_(userfs) {
    if (rootfs_ == nullptr) {
        throw std::runtime_error("Cannot open title storage.");
    }
    if (userfs_ == nullptr) {
        throw std::runtime_error("Cannot open user storage.");
    }
    while (SDL_StorageReady(rootfs_)) {
        SDL_Delay(1);
    }
    while (SDL_StorageReady(userfs_)) {
        SDL_Delay(1);
    }
}

void Context::setState(State v) {
    // TODO
    state_ = v;
}

VOLCANO_FRAMEWORK_END
