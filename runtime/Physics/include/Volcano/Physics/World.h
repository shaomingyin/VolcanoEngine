//
//
#ifndef VOLCANO_PHYSICS_WORLD_H
#define VOLCANO_PHYSICS_WORLD_H

#include <memory>
#include <type_traits>

#include <Volcano/World/Inherency.h>
#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class World {
public:
	World(entt::registry& registry);
	World(const World&) = delete;
	World(World&&) = delete;
	virtual ~World();

public:
	World& operator=(const World&) = delete;
	World& operator=(World&&) = delete;

	void enable();
	void disable();

	bool isEnable() const {
		return bool(bt_dynamics_world_);
	}

	void frame(Duration elapsed) {
		if (bt_dynamics_world_) {
			bt_dynamics_world_->stepSimulation(durationToMicroseconds(elapsed) / 1000000.0);
		}
	}

	Eigen::Vector3f gravity() const {
		auto v = bt_dynamics_world_->getGravity();
		return Eigen::Vector3f(v.x(), v.y(), v.z());
	}

	void setGravity(const Eigen::Vector3f& v) {
		bt_dynamics_world_->setGravity(btVector3(v.x(), v.y(), v.z()));
	}

private:
	void addRigidBody(RigidBody& body, Volcano::World::Inherency* inherency = nullptr);

	template <typename T>
	void onRigidBodyAdded(entt::registry& registry, entt::entity entity) {
		VOLCANO_ASSERT(&registry_ == &registry);
		static_assert(std::is_base_of_v<RigidBody, T>);
		if (bt_dynamics_world_ && registry.valid(entity)) {
			addRigidBody(
				registry.get<T>(entity),
				registry.try_get<Volcano::World::Inherency>(entity));
		}
	}

	template <typename T>
	void onRigidBodyRemoved(entt::registry& registry, entt::entity entity) {
		VOLCANO_ASSERT(&registry_ == &registry);
		static_assert(std::is_base_of_v<RigidBody, T>);
		if (bt_dynamics_world_) {
			bt_dynamics_world_->removeRigidBody(&registry.get<T>(entity));
		}
	}

private:
	entt::registry& registry_;
	Eigen::Vector3f gravity_;
	std::unique_ptr<btDefaultCollisionConfiguration> bt_collision_configuration_;
	std::unique_ptr<btCollisionDispatcher>  bt_collision_dispatcher_;
	std::unique_ptr<btBroadphaseInterface> bt_overlapping_pair_cache_;
	std::unique_ptr<btSequentialImpulseConstraintSolver> bt_sequential_impulse_constraint_solver_;
	std::unique_ptr<btDynamicsWorld> bt_dynamics_world_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_WORLD_H
