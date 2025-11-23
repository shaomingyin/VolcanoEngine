//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <chrono>
#include <string>
#include <vector>

#include <Volcano/Version.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

class World {
    RTTR_ENABLE()

public:
    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Error
    };

    using Clock = Context::Clock;
    using Entities = std::vector<Entity>;

public:
    World(Context& context);
    virtual ~World();

public:
    Context& context() noexcept {
        return context_;
    }

    State state() const noexcept {
        return state_;
    }

    void load();

    bool isPhysicsEnabled() const noexcept {
        return physics_enabled_;
    }

    void enablePhysics() noexcept {
        physics_enabled_ = true;
    }

    void disablePhysics() noexcept {
        physics_enabled_ = false;
    }

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

    const Entities& entities() const noexcept {
        return entities_;
    }

    Entity& emplace() {
        return entities_.emplace_back(context_);
    }

    Entity& global() noexcept {
        return global_;
    }

    const Entity& global() const noexcept {
        return global_;
    }

    Entity& avatar() noexcept {
        return avatar_;
    }

    const Entity& avatar() const noexcept {
        return avatar_;
    }

    virtual void stepSimulation(Clock::duration elapsed);

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

    bool physics_enabled_;
	Eigen::Vector3f gravity_;
    btDefaultCollisionConfiguration bt_config_;
    btCollisionDispatcher bt_dispatcher_;
    btDbvtBroadphase bt_broadphase_;
    btSequentialImpulseConstraintSolver bt_solver_;
    btDiscreteDynamicsWorld bt_world_;

    Entities entities_;
    Entity global_;
    Entity avatar_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
