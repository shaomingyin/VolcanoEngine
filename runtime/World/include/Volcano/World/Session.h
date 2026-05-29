//
//
#ifndef VOLCANO_WORLD_SESSION_H
#define VOLCANO_WORLD_SESSION_H

#include <rttr/type>

#include <Volcano/World/Common.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

class Session {
    RTTR_ENABLE()

public:
    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Playing,
        Paused,
        Error,

        Max
    };

public:
    Session();
    virtual ~Session();

public:
    State state() const noexcept {
        return state_;
    }

    Scene& scene() noexcept {
        scene(state_);
    }

    const Scene& scene() const noexcept {
        scene(state_);
    }

    Scene& scene(State st) noexcept {
        return scenes_[static_cast<size_t>(st)];
    }

    const Scene& scene(State st) const noexcept {
        return scenes_[static_cast<size_t>(st)];
    }

    virtual void update(Clock::duration elapsed) noexcept;

private:
    State state_;
    Scene scenes_[static_cast<size_t>(State::Max)];
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SESSION_H
