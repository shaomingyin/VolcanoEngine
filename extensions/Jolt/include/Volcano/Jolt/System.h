//
//
#ifndef VOLCANO_JOLT_SYSTEM_H
#define VOLCANO_JOLT_SYSTEM_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Volcano/Physics/System.h>
#include <Volcano/Jolt/Common.h>

VOLCANO_JOLT_BEGIN

class System : public Physics::System {
public:
	System(World::Scene& scene);
	~System() override;

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

#endif // VOLCANO_JOLT_SYSTEM_H
