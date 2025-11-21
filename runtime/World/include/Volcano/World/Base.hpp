//
//
#ifndef VOLCANO_WORLD_BASE_HPP
#define VOLCANO_WORLD_BASE_HPP

#include <array>
#include <chrono>
#include <string>
#include <vector>
#include <filesystem>

#include <SFML/Window/Event.hpp>

#include <Volcano/Version.hpp>
#include <Volcano/World/Common.hpp>
#include <Volcano/World/Scene.hpp>
#include <Volcano/World/Context.hpp>

VOLCANO_WORLD_BEGIN

class Base {
public:
    using Clock = Context::Clock;

    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Paused,
        Error
    };

public:
    Base(Context& context);
    virtual ~Base();

public:
    Context& context() noexcept {
        return context_;
    }

    State state() const noexcept {
        return state_;
    }

    unsigned int loadingProgress() const noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        return loading_progress_;
    }

    const std::string& loadingMessage() const noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        return loading_message_;
    }

    const std::error_code& error() const noexcept {
        VOLCANO_ASSERT(state_ == State::Error);
        return error_;
    }

protected:
    void setLoadingCount(unsigned int count) noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        loading_count_ = count;
    }

    void setLoadingProgress(unsigned int progress) noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        loading_progress_ = std::clamp(progress, (unsigned int)0, loading_count_);
    }

    void setLoadingMessage(std::string message) noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        loading_message_ = std::move(message);
    }

    void setReady() noexcept {
		VOLCANO_ASSERT(state_ == State::Loading);
        state_ = State::Ready;
	}

    void setError(std::error_code ec) noexcept {
        state_ = State::Error;
        error_ = ec;
    }

private:
    Context& context_;
    State state_;
    unsigned int loading_count_;
    unsigned int loading_progress_;
    std::string loading_message_;
    std::error_code error_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BASE_HPP
