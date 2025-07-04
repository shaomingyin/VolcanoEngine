/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_GAME_H
#define VOLCANO_FRAMEWORK_GAME_H

#include <string>

#include <nlohmann_json.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>

#include <Volcano/VersionNumber.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

class Game {
public:
    enum class State {
        Idle = 0,
        Loading,
        Aborting,
        Ready,
        Playing,
        Paused,
        Stopped,
        Error
    };

public:
    Game(Context& context);
    virtual ~Game() = default;

public:
    Context& context() noexcept {
        return context_;
    }

    virtual const std::string& name() const { return name_; /* FIXME */}

    State state() const noexcept {
        return state_;
    }

    World::Scene& scene() noexcept {
        return scene_;
    }

    const World::Scene& scene() const noexcept {
        return scene_;
    }
    
    void quit() noexcept {
        quit_ = true;
    }

    void load(std::string scene_path);
    void abortLoading();
    float loadingProgress() const noexcept;

protected:
    void setState(State v) noexcept;

    void loadingCheckPoint() const {
        async::interruption_point(loading_cancellation_token_);
    }

    void setLoadingTotal(uint64_t v) noexcept {
        loading_total_ = v;
    }

    void setLoadingProgress(uint64_t v) noexcept {
        loading_progress_ = v;
    }

    void setError(std::string message) noexcept {
        error_message_ = std::move(message);
        setState(State::Error);
    }

    bool isQuit() const noexcept {
        return quit_;
    }

    virtual void loadConfig(const nlohmann::json& j);
    virtual void loadScene(const nlohmann::json& j);

private:
    Context& context_;
    
    std::string name_;
    State state_;
    bool quit_;

    std::string scene_path_;
    World::Scene scene_;

    async::task<void> loading_task_;
    async::cancellation_token loading_cancellation_token_;
    uint64_t loading_total_;
    uint64_t loading_progress_;

    std::string error_message_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_GAME_H
