//
//
#ifndef VOLCANO_SIMULATION_WORLD_HPP
#define VOLCANO_SIMULATION_WORLD_HPP

#include <chrono>
#include <string>
#include <vector>
#include <filesystem>

#include <Volcano/Version.hpp>
#include <Volcano/Simulation/Common.hpp>
#include <Volcano/Simulation/Light.hpp>
#include <Volcano/Simulation/Scene.hpp>
#include <Volcano/Simulation/Context.hpp>

VOLCANO_SIMULATION_BEGIN

class World {
public:
    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Error
    };

    using Clock = Context::Clock;
    using Scenes = std::vector<Scene>;

public:
    World(Context& context);
    virtual ~World();

public:
    Context& context() noexcept {
        return context_;
    }

    virtual const VersionNumber& version() const noexcept = 0;
    virtual const std::string& name() const noexcept = 0;

    State state() const noexcept {
        return state_;
    }

    void load();

    const Light& ambientLight() const noexcept {
        return ambient_light_;
    }

    const Scenes& scenes() const noexcept {
        return scenes_;
    }

    entt::entity player() const noexcept {
        return player_;
    }

    bool isPhysicsEnabled() const noexcept {
        return physics_enabled_;
    }

    void enablePhysics() noexcept;
    void disablePhysics() noexcept;

    Eigen::Vector3f getGravity() const noexcept {
		return gravity_;
    }

	void setGravity(const Eigen::Vector3f& v) noexcept;

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

    virtual void update(Clock::duration elapsed);

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

    Light ambient_light_;
    entt::entity player_;

    Scenes scenes_;
    bool physics_enabled_;
	Eigen::Vector3f gravity_;

    unsigned int loading_count_;
    unsigned int loading_progress_;
    std::string loading_message_;

    std::error_code error_;
};

VOLCANO_SIMULATION_END

#endif // VOLCANO_SIMULATION_WORLD_HPP
