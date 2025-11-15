//
//
#ifndef VOLCANO_FRAMEWORK_WORLD_HPP
#define VOLCANO_FRAMEWORK_WORLD_HPP

#include <array>
#include <chrono>
#include <string>
#include <filesystem>

#include <SFML/Window/Event.hpp>

#include <Volcano/Version.hpp>
#include <Volcano/Vm/Kernel.hpp>
#include <Volcano/Scene/Registry.hpp>
#include <Volcano/Framework/Context.hpp>

VOLCANO_FRAMEWORK_BEGIN

class World: public Vm::Kernel {
public:
    using Clock = Context::Clock;

    enum class State {
        Loading = 0,
        Ready,
        Error
    };

public:
    World(Context& context, const std::filesystem::path& initrc = "/init.lua");
    virtual ~World();

public:
    Context& context() noexcept {
        return context_;
    }

    const VersionNumber& version() const noexcept {
        return version_;
    }

    const std::string& name() const noexcept {
        return name_;
    }

    State state() const noexcept {
        return state_;
    }

    void play() noexcept;
	void pause() noexcept;
	void postEvent(const sf::Event& evt) noexcept;

    bool isPhysicsEnabled() const noexcept {
        return physics_enabled_;
    }

    void enablePhysics() noexcept;
    void disablePhysics() noexcept;

    Eigen::Vector3f getGravity() const noexcept {
		return gravity_;
    }

	void setGravity(const Eigen::Vector3f& g) noexcept;

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

    unsigned long fps() const noexcept {
		return frame_count_per_second_;
	}

	unsigned long fpsMax() const noexcept {
        return (1000000000 / std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_min_).count());
	}

    void setFpsMax(unsigned long v) noexcept;

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

	void mainLoop(lua_State* L) override;
    void scheduleTrappedTasks(lua_State* L) override;
	virtual void frame(Clock::duration elapsed);
	virtual void handleEvent(const sf::Event& evt);

private:
	void setupFrameTimer();
    void processEvents();
    void updateFrame();

private:
    Context& context_;
    State state_;
    std::string name_;
    VersionNumber version_;

    uv_timer_t frame_timer_;
	Clock::duration elapsed_min_;
	Clock::time_point frame_last_;
    unsigned int frame_count_;
    unsigned int frame_count_per_second_;

    Scene::Registry scene_;
    bool physics_enabled_;
	Eigen::Vector3f gravity_;

    unsigned int loading_count_;
    unsigned int loading_progress_;
    std::string loading_message_;

    std::error_code error_;

	static constexpr size_t EventQueueSize = (1 << 8);
	static constexpr size_t EventQueueMask = EventQueueSize - 1;
	std::array<sf::Event, EventQueueSize> event_queue_;
    size_t event_queue_start_;
	size_t event_queue_end_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_WORLD_HPP
