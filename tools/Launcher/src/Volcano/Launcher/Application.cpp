/*
 *
 */
#include <Volcano/Launcher/Application.h>

VOLCANO_LAUNCHER_BEGIN

Application::Application(rttr::type game_type, SDL_Storage* rootfs, SDL_Storage* userfs)
    : Framework::Context(rootfs, userfs)
    , game_type_(game_type)
    , frame_last_(Clock::now()) {
    auto game_inst = game_type_.create({ *this });
    if (!game_inst.is_type<Framework::Game>()) {
        throw std::invalid_argument("Invalid game type.");
    }
    game_.reset(game_inst.get_value<Framework::Game*>());
    setState(State::Loading);
    loading_task_ = async::spawn([this] {
        try {
            loadConfig(nlohmann::loadFromStorage(this->rootfs(), "Config.json"));
            loadConfig(nlohmann::loadFromStorage(this->userfs(), "Config.json"));
        } catch (std::exception& e) {
            logWarning("{}", e.what());
        }
        loadScene(nlohmann::loadFromStorage(this->rootfs(), "Scenes/Startup.json"));
    }).then(*this, [this] {
        setState(State::Ready);
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
    setElapsed(curr - frame_last_);
    frame_last_ = curr;

    switch (state()) {
    case State::Playing:
        game_->playginFrame();
        ret = SDL_APP_CONTINUE;
        break;
    case State::Loading:
        game_->loadingFrame();
        ret = SDL_APP_CONTINUE;
        break;
    case State::Ready:
        game_->readyFrame();
        ret = SDL_APP_CONTINUE;
        break;
    case State::Error:
        game_->errorFrame();
        ret = SDL_APP_CONTINUE;
        break;
    default:
        ret = SDL_APP_CONTINUE;
        break;
    }

    return ret;
}

void Application::loadConfig(const nlohmann::json& j) {
    game_->loadConfig(j);
}

void Application::loadScene(const nlohmann::json& j) {
    game_->loadScene(j);
}

VOLCANO_LAUNCHER_END
