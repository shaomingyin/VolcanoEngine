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

enum class State {
    Idle = 0,
    Loading,
    Ready,
    Playing,
    Paused,
    Stopped,
    Error
};

template <typename T>
concept Context_state = requires(const T & v) {
    { v.state() } -> std::same_as<State>;
};

template <typename T>
concept Context_load = requires(T & v, const std::string & scene_path) {
    v.load(scene_path);
};

template <typename T>
concept Context_event = requires(T & v, const SDL_Event & evt) {
    v.event(evt);
};

template <typename T>
concept Context_schedule = requires(T & v, async::task_run_handle t) {
    v.schedule(std::move(t));
};

template <typename T>
concept Context_quit = requires(T & v) {
    v.quit();
};

template <typename T>
concept Context_rootfs = requires(T & v) {
    { v.rootfs() } -> std::same_as<SDL_Storage*>;
};

template <typename T>
concept Context_userfs = requires(T & v) {
    { v.userfs() } -> std::same_as<SDL_Storage*>;
};

template <typename T>
concept Context_scene = requires(T & v) {
    { v.scene() } -> std::convertible_to<World::Scene&>;
};

protected:
    void setState(State v);

    void setError(std::string message) {
        error_message_ = std::move(message);
        setState(State::Error);
    }

    virtual void loadConfig(const nlohmann::json& j);
    virtual void loadScene(const nlohmann::json& j);
    virtual void frame(Duration elapsed);

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
    TimePoint frame_last_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CONTEXT_H
