//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <chrono>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
public:
    using Clock = std::chrono::steady_clock;

public:
    Scene();
    virtual ~Scene();

public:
    void update(Clock::duration elapsed) noexcept;

private:
    void onCollisionUpdate(entt::registry&, entt::entity ent) noexcept;
    JPH::Shape* createShape(const Collision& collision);

private:
    JPH::PhysicsSystem physics_system_;
    JPH::BodyInterface& body_interface_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
