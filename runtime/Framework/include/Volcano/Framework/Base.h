//
//
#ifndef VOLCANO_FRAMEWORK_BASE_H
#define VOLCANO_FRAMEWORK_BASE_H

#include <string>
#include <memory>
#include <functional>

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/World.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

class Base: public Context {
public:
    using WorldCreator = std::function<std::unique_ptr<World>(Context& context)>;

public:
    Base(WorldCreator world_creator);
    ~Base() override = default;

public:
    void run();
    void quit() noexcept override;
    unsigned long fps() const noexcept override;
    unsigned long fpsMax() const noexcept override;
    void setFpsMax(unsigned long v) noexcept override;

protected:
    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Playing,
        Paused,
        Error
    };

    World& world() noexcept {
        VOLCANO_ASSERT(world_);
        return *world_;
    }

    std::error_code error() const noexcept {
        VOLCANO_ASSERT(state_ == State::Error);
        return error_;
    }

    void play() noexcept;
    void pause() noexcept;
    void setError(std::error_code ec) noexcept;

    virtual void frame(Clock::duration elapsed) noexcept;
    virtual void loadingFrame(Clock::duration elapsed) noexcept;
    virtual void readyFrame(Clock::duration elapsed) noexcept;
    virtual void playingFrame(Clock::duration elapsed) noexcept;
    virtual void pausedFrame(Clock::duration elapsed) noexcept;
    virtual void errorFrame(Clock::duration elapsed) noexcept;

private:
    State state_;
    WorldCreator world_creator_;
    Clock::duration min_elapsed_;
    Clock::time_point frame_last_;
    Clock::time_point frame_count_last_;
    unsigned long frame_count_;
    unsigned long frame_count_per_second_;
    std::unique_ptr<World> world_;
    std::error_code error_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_BASE_H
