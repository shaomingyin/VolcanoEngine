//
//
#ifndef VOLCANO_WORLD_PHYSICS_H
#define VOLCANO_WORLD_PHYSICS_H

#include <entt/entt.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Physics {
public:
    Physics(entt::registry& registry);
    virtual ~Physics();

public:
    bool isEnabled() const noexcept {
        return enabled_;
    }

    void enable() noexcept;
    void disable() noexcept;

    const Eigen::Vector3f& gravity() const noexcept {
        return gravity_;
    }

    void setGravity(const Eigen::Vector3f& v) noexcept;
    void update(Clock::duration elapsed) noexcept;

protected:
    void onBodyAdded(entt::entity ent) noexcept;
    void onBodyRemoved(entt::entity ent) noexcept;

private:
    entt::registry& registry_;
    bool enabled_;
    Eigen::Vector3f gravity_;
    JPH::PhysicsSystem physics_system_;
    JPH::BodyInterface* body_interface_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_PHYSICS_H
