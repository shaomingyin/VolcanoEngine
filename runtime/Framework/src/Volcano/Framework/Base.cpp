//
//
#include <SDL3/SDL_timer.h>

#include <Volcano/Framework/ContextImpl.h>

VOLCANO_FRAMEWORK_BEGIN

ContextImpl::ContextImpl(SDL_Storage* rootfs, SDL_Storage* userfs, rttr::type game_type)
    : Context(rootfs, userfs) {
    VOLCANO_ASSERT(game_type.is_derived_from<Game>());
    game_instance_ = game_type.create({ *this });
    if (!game_instance_) {
        throw std::runtime_error("Failed to create game instance.");
    }
    game_ = &game_instance_.get_value<Game>();
}

void ContextImpl::event(const SDL_Event& evt) {
    Context::event(evt);
    VOLCANO_ASSERT(game_ != nullptr);
    game_->event(evt);
}

void ContextImpl::frame(Duration elapsed) {
    Context::frame(elapsed);

    switch (state()) {
    case State::Playing:
        physics_.update(elapsed);
        break;
    default:
        break;
    }

    VOLCANO_ASSERT(game_ != nullptr);
    game_->frame(elapsed);
}

void ContextImpl::loadConfig(const nlohmann::json& j) {
    Context::loadConfig(j);
    VOLCANO_ASSERT(game_ != nullptr);
    game_->loadConfig(j);
}

void ContextImpl::loadScene(const nlohmann::json& j) {
    Context::loadScene(j);
    VOLCANO_ASSERT(game_ != nullptr);
    game_->loadScene(j);
}

VOLCANO_FRAMEWORK_END
