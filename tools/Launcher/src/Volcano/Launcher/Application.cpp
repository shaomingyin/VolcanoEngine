/*
 *
 */
#include <Volcano/Launcher/Application.h>

VOLCANO_LAUNCHER_BEGIN

Application::Application(SDL_Storage* rootfs, SDL_Storage* userfs)
    : Framework::Context(rootfs, userfs)
    , frame_last_(Clock::now()) {
    loadAcousticsSpace();
    loadGraphicsRenderer();
    loadPhysicsWorld();
    loadGame();
    setState(State::Loading);
    loading_task_ = async::spawn([this] {
        try {
            loadConfig(nlohmann::loadFromStorage(this->rootfs(), "Config.json"));
            loadConfig(nlohmann::loadFromStorage(this->userfs(), "Config.json"));
        } catch (std::exception& e) {
            logWarning("{}", e.what());
        }
        loadScene(nlohmann::loadFromStorage(this->rootfs(), "Scenes/Startup.json"));
    });
}

SDL_AppResult Application::event(const SDL_Event& evt) {
    SDL_AppResult ret;

    switch (state()) {
    case State::Playing:
        if (graphics_window_) {
            graphics_window_->handleEvent(evt);
        }
        game_->event(evt);
        ret = SDL_APP_CONTINUE;
        break;
    case State::Loading:
        ret = SDL_APP_CONTINUE;
        break;
    case State::Ready:
        ret = SDL_APP_CONTINUE;
        break;
    case State::Error:
        ret = SDL_APP_CONTINUE;
        break;
    default:
        ret = SDL_APP_CONTINUE;
        break;
    }

    return ret;
}

SDL_AppResult Application::update() {
    SDL_AppResult ret;

    auto curr = Clock::now();
    auto elapsed = curr - frame_last_;
    frame_last_ = curr;

    switch (state()) {
    case State::Playing:
        ret = SDL_APP_CONTINUE;
        break;
    case State::Loading:
        if (loading_task_.canceled()) {
            setState(State::Idle);
        } else if (loading_task_.ready()) {
            setState(State::Ready);
        } else {
        }
        ret = SDL_APP_CONTINUE;
        break;
    case State::Ready:
        ret = SDL_APP_CONTINUE;
        break;
    case State::Error:
        ret = SDL_APP_CONTINUE;
        break;
    default:
        ret = SDL_APP_CONTINUE;
        break;
    }

    game_->frame(elapsed);

    return ret;
}

void Application::loadConfig(const nlohmann::json& j) {
    game_->loadConfig(j);
}

void Application::loadScene(const nlohmann::json& j) {
    game_->loadScene(j);
}

void Application::loadAcousticsSpace() {
    auto types = rttr::type::get<Acoustics::Space>().get_derived_classes();
    if (!types.empty()) {
        auto object = types.begin()->create();
        if (object) {
            acoustics_space_.reset(object.get_value<Acoustics::Space*>());
        }
    }
}

void Application::loadGraphicsRenderer() {
    auto types = rttr::type::get<Acoustics::Space>().get_derived_classes();
    if (!types.empty()) {
        auto object = types.begin()->create();
        if (object) {
            acoustics_space_.reset(object.get_value<Acoustics::Space*>());
        }
    }
}

void Application::loadPhysicsWorld() {
    auto types = rttr::type::get<Acoustics::Space>().get_derived_classes();
    if (!types.empty()) {
        auto object = types.begin()->create();
        if (object) {
            acoustics_space_.reset(object.get_value<Acoustics::Space*>());
        }
    }
}

void Application::loadGame() {
    auto types = rttr::type::get<Framework::Game>().get_derived_classes();
    if (!types.empty()) {
        auto object = types.begin()->create({ *this });
        if (object) {
            game_.reset(object.get_value<Framework::Game*>());
        }
    }
}

VOLCANO_LAUNCHER_END
