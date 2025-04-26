//
//
#ifndef VOLCANO_FRAMEWORK_CONTEXT_H
#define VOLCANO_FRAMEWORK_CONTEXT_H

#include <SDL3/SDL_storage.h>

#include <entt/entt.hpp>

#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

class VOLCANO_FRAMEWORK_API Context {
public:
    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Playing,
        Paused,
        Error
    };

public:
    Context(SDL_Storage* rootfs, SDL_Storage* userfs);
    virtual ~Context() = default;

public:
    State state() const {
        return state_;
    }

    void schedule(async::task_run_handle t) {
        scheduler_.schedule(std::forward<async::task_run_handle>(t));
    }

    SDL_Storage* rootfs() {
        return rootfs_;
    }

    SDL_Storage* userfs() {
        return userfs_;
    }

    entt::registry& registry() {
        return registry_;
    }

    Duration elapsed() const {
        return elapsed_;
    }

protected:
    void runAllTasks() {
        scheduler_.run_all_tasks();
    }

    void setElapsed(Duration v) {
        elapsed_ = v;
    }

    void setState(State v);

private:
    State state_;
    async::fifo_scheduler scheduler_;
    SDL_Storage* rootfs_;
    SDL_Storage* userfs_;
    entt::registry registry_;
    Duration elapsed_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CONTEXT_H
