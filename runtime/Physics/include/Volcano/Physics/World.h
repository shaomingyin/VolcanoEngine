//
//
#ifndef VOLCANO_PHYSICS_WORLD_H
#define VOLCANO_PHYSICS_WORLD_H

#include <Volcano/Math.h>
#include <Volcano/Stage/Scene.h>
#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

class World {
public:
	using Clock = Stage::Scene::Clock;

public:
	World(Stage::Scene& scene);
	virtual ~World();

public:
	bool isEnabled() const noexcept {
		return enabled_;
	}

	void enable() noexcept;
	void disable() noexcept;

	const Eigen::Vector3f& gravity() const noexcept {
		return gravity_;
	}

	void setGravity(const Eigen::Vector3f& g) noexcept;

	void update(Clock::duration elapsed) noexcept;

protected:
	virtual void onGravityChanged(const Eigen::Vector3f& v) noexcept;
	virtual void onEnabledChanged(bool enabled) noexcept;
	virtual void onUpdate(Clock::duration elapsed) noexcept;

protected:
	virtual void onBoxCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept;
	virtual void onSphereCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept;
	virtual void onCapsuleCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept;
	virtual void onPlaneCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept;
	virtual void onMeshCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept;

private:
	Stage::Scene& scene_;
	bool enabled_;
	Eigen::Vector3f gravity_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_WORLD_H
