//
//
#include <SDL3/SDL_timer.h>

#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

Context::Context(SDL_Storage* rootfs, SDL_Storage* userfs)
    : state_(State::Idle)
    , rootfs_(rootfs)
    , userfs_(userfs) {
    VOLCANO_ASSERT(rootfs_ != nullptr);
    VOLCANO_ASSERT(SDL_StorageReady(rootfs_));
    VOLCANO_ASSERT(userfs_ != nullptr);
    VOLCANO_ASSERT(SDL_StorageReady(userfs_));
}

void Context::setState(State v) {
    // TODO
    state_ = v;
}

VOLCANO_FRAMEWORK_END
