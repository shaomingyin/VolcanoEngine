//
//
#ifndef VOLCANO_JOLT_WORLD_H
#define VOLCANO_JOLT_WORLD_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Volcano/Physics/World.h>
#include <Volcano/Jolt/Common.h>

VOLCANO_JOLT_BEGIN

class World : public Physics::World {
public:
	World(Stage::Scene& scene);
	~World() override;

protected:
	void onGravityChanged(const Eigen::Vector3f& v) noexcept override;
	void onEnabledChanged(bool enabled) noexcept override;
	void onUpdate(Clock::duration elapsed) noexcept override;

protected:
	void onBoxCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept override;
	void onSphereCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept override;
	void onCapsuleCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept override;
	void onPlaneCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept override;
	void onMeshCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept override;

private:
	JPH::PhysicsSystem physics_system_;
};

VOLCANO_JOLT_END

#endif // VOLCANO_JOLT_WORLD_H
