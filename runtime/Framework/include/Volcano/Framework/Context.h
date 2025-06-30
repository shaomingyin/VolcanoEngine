//
//
#ifndef VOLCANO_FRAMEWORK_CONTEXT_H
#define VOLCANO_FRAMEWORK_CONTEXT_H

#include <memory>

#include <rttr/type>
#include <entt/entt.hpp>
#include <nlohmann_json.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_storage.h>

#include <Volcano/World/Scene.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

class Context {
public:
    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Playing,
        Paused,
        Stopped,
        Error
    };

public:
    Context(SDL_Storage* rootfs, SDL_Storage* userfs);
    virtual ~Context() = default;

public:
    void load(std::string scene_path);

    State state() const noexcept {
        return state_;
    }

    SDL_Storage* rootfs() noexcept {
        return rootfs_;
    }

    SDL_Storage* userfs() noexcept {
        return userfs_;
    }

    World::Scene& scene() {
        return scene_;
    }

    void schedule(async::task_run_handle t) {
        scheduler_.schedule(std::move(t));
    }

    void quit() noexcept {
        quit_ = true;
    }

protected:
    void setState(State v);

    void setError(std::string message) {
        error_message_ = std::move(message);
        setState(State::Error);
    }

    void runAllTasks() {
        scheduler_.run_all_tasks();
    }

    bool isQuit() const noexcept {
        return quit_;
    }

    virtual void loadConfig(const nlohmann::json& j);
    virtual void loadScene(const nlohmann::json& j);

private:
    State state_;
    bool quit_;
    async::fifo_scheduler scheduler_;
    async::task<void> loading_task_;
    async::cancellation_token loading_cancellation_token_;
    std::string error_message_;
    SDL_Storage* rootfs_;
    SDL_Storage* userfs_;
    World::Scene scene_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CONTEXT_H
