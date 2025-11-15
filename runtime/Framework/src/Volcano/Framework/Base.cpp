//
//
#include <thread>
#include <stdexcept>
#include <filesystem>

#include <Volcano/Error.hpp>
#include <Volcano/Framework/Base.hpp>

VOLCANO_FRAMEWORK_BEGIN

using namespace std::chrono_literals;

Base::Base(WorldCreator world_creator)
    : state_(State::Idle)
    , world_creator_(std::move(world_creator))
    , frame_last_(Clock::now())
    , frame_count_last_(frame_last_)
    , frame_count_(0)
    , frame_count_per_second_(0) {
    VOLCANO_ASSERT(world_creator);
}

void Base::run() {
    VOLCANO_ASSERT(state_ == State::Idle);

    world_task_ = async::spawn(async::thread_scheduler(), [this]() {
        state_ = State::Loading;
        world_creator_(*this)->run();
    });

    while (!world_task_.ready()) {
        auto now = Clock::now();
        if ((now - frame_count_last_) >= 1s) {
            frame_count_per_second_ = frame_count_;
            frame_count_ = 0;
            frame_count_last_ = now;
        }
        frame(now - frame_last_);
        auto update_elapsed = Clock::now() - now;
        if (update_elapsed < min_elapsed_) {
            std::this_thread::sleep_for(min_elapsed_ - update_elapsed);
        }
    }
}

unsigned long Base::fps() const noexcept {
    return frame_count_per_second_;
}

unsigned long Base::fpsMax() const noexcept {
    return (1000000000ns / min_elapsed_);
}

void Base::setFpsMax(unsigned long v) noexcept {
    if (v < 1) {
        v = 1;
    }
    min_elapsed_ = (1000000000ns / v);
}

void Base::play() noexcept {
    if (state_ == State::Ready || state_ == State::Paused) {
        state_ = State::Playing;
    }
}

void Base::pause() noexcept {
    if (state_ == State::Playing) {
        state_ = State::Paused;
    }
}

void Base::setError(std::error_code ec) noexcept {
    state_ = State::Error;
    error_ = ec;
}

void Base::frame(Clock::duration elapsed) noexcept {
    runAllTasks();

    switch (state_) {
    case State::Playing:
        playingFrame(elapsed);
        break;
    case State::Paused:
        pausedFrame(elapsed);
        break;
    case State::Loading:
        loadingFrame(elapsed);
        break;
    case State::Ready:
        readyFrame(elapsed);
        break;
    case State::Error:
        errorFrame(elapsed);
        break;
    default:
        break;
    }
}

void Base::loadingFrame(Clock::duration elapsed) noexcept {
}

void Base::readyFrame(Clock::duration elapsed) noexcept {
}

void Base::playingFrame(Clock::duration elapsed) noexcept {
}

void Base::pausedFrame(Clock::duration elapsed) noexcept {
}

void Base::errorFrame(Clock::duration elapsed) noexcept {
}

VOLCANO_FRAMEWORK_END
