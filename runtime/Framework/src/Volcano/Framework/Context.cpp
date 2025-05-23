//
//
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

Context::Context(SDL_Storage* rootfs, SDL_Storage* userfs)
    : state_(State::Idle)
    , quit_(false)
    , rootfs_(rootfs)
    , userfs_(userfs) {
    VOLCANO_ASSERT(rootfs_ != nullptr);
    VOLCANO_ASSERT(SDL_StorageReady(rootfs_));
    VOLCANO_ASSERT(userfs_ != nullptr);
    VOLCANO_ASSERT(SDL_StorageReady(userfs_));
    auto physics_world_types = rttr::type::get<Physics::World>().get_derived_classes();
    if (physics_world_types.empty()) {
        return;
    }
    physics_world_instance_ = physics_world_types.begin()->create({ scene_ });
    if (!physics_world_instance_) {
        return;
    }
    physics_world_ = &physics_world_instance_.get_value<Physics::World>();
}

void Context::load(std::string scene_path) {
    VOLCANO_ASSERT(state_ == State::Idle);
    setState(State::Loading);

    try {
        loadConfig(nlohmann::loadFromStorage(rootfs_, "/Config.json"));
        loadConfig(nlohmann::loadFromStorage(userfs_, "/Config.json"));
    } catch (...) {
    }

    loading_task_ = async::spawn([this, scene_path = std::move(scene_path)] {
        loadScene(nlohmann::loadFromStorage(rootfs_, scene_path.c_str()));
    }).then(scheduler_, [this](async::task<void> t) {
        try {
            t.get();
            setState(State::Ready);
        } catch (async::task_canceled e) {
            setState(State::Idle);
        } catch (std::exception e) {
            setError(e.what());
        }
    });
}

void Context::event(const SDL_Event& evt) {
}

SDL_AppResult Context::update() {
    auto curr = Clock::now();
    frame(curr - frame_last_);
    frame_last_ = curr;
    if (quit_) {
        if (state_ == State::Error) {
            return SDL_APP_FAILURE;
        }
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

void Context::setState(State v) {
    // TODO
    state_ = v;
}

void Context::loadConfig(const nlohmann::json& j) {
    // TODO load physics comp.
}

void Context::loadScene(const nlohmann::json& j) {
}

void Context::frame(Duration elapsed) {
    scheduler_.run_all_tasks();
}

VOLCANO_FRAMEWORK_END
