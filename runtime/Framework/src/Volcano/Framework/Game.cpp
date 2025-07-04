//
//
#include <Volcano/Framework/Game.h>

VOLCANO_FRAMEWORK_BEGIN

Game::Game(Context& context)
    : context_(context)
    , state_(State::Idle) {
}

void Game::load(std::string scene_path) {
    if (state_ == State::Loading) {
        throw std::runtime_error("Cannot start loading while loading.");
    }

    scene_path = std::move(scene_path);
    loading_task_ = async::spawn([this] {
        while (!SDL_StorageReady(rootfs_)) {
            loadingCheckPoint();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        loadScene(nlohmann::loadFromStorage(rootfs_, scene_path_.c_str()));
    }).then(*this, [this](async::task<void> task) {
        try {
            task.get();
            setState(State::Ready);
        } catch (const async::task_canceled& e) {
            setState(State::Idle);
        } catch (const std::exception& e) {
            setError(e.what());
        }
    });

    setState(State::Loading);
}

void Game::abortLoading() {
    if (state_ == State::Loading) {
        loading_cancellation_token_.cancel();
        setState(State::Aborting);
    }
}

float Game::loadingProgress() const noexcept {
    if (state_ == State::Loading) {
        if (loading_progress_ <= loading_total_) {
            return (loading_progress_ * 100.0f / loading_total_);
        }
        return 1.0f;
    }
    return 0.0f;
}

void Game::setState(State v) noexcept {
    // TODO
    state_ = v;
}

void Game::loadConfig(const nlohmann::json& j) {
}

void Game::loadScene(const nlohmann::json& j) {
}

VOLCANO_FRAMEWORK_END
