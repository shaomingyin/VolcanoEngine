//
//
#ifndef VOLCANO_GAME_WORLD_H
#define VOLCANO_GAME_WORLD_H

#include <memory>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Entity.h>
#include <Volcano/Game/Light.h>
#include <Volcano/Game/Camera.h>

VOLCANO_GAME_BEGIN

class World {
public:
	World();
	World(const World&) = delete;
	World(World&&) = delete;
	virtual ~World();

public:
	World& operator=(const World&) = delete;
	World& operator=(World&&) = delete;

	void update(Duration elapsed);

	[[nodiscard]] Entity createEntity(std::string name = std::string()) {
		return Entity(registry_, std::move(name));
	}

	template <typename Component>
	auto view() {
		return registry_.view<Component>();
	}

	template <typename... Components>
	auto view() {
		return registry_.view<Components...>();
	}

	Light& ambientLight() {
		return registry_.get_or_emplace<Light>(global_);
	}

	const Light& ambientLight() const {
		return registry_.get<Light>(global_);
	}

	Camera& camera() {
		return registry_.get_or_emplace<Camera>(global_);
	}

	const Camera& camera() const {
		return registry_.get<Camera>(global_);
	}

	bool isPhysicsEnabled() {
		return physics_enabled_;
	}

	void enablePhysics() {
		physics_enabled_ = true;
	}

	void disablePhysics() {
		physics_enabled_ = false;
	}

	Eigen::Vector3f gravity() const {
		btVector3 v = bt_dynamics_world_->getGravity();
		return Eigen::Vector3f(v.x(), v.y(), v.z());
	}

	void setGravity(Eigen::Vector3f v) {
		bt_dynamics_world_->setGravity(btVector3(v.x(), v.y(), v.z()));
	}

protected:
	entt::registry& registry() {
		return registry_;
	}

	const entt::registry& registry() const {
		return registry_;
	}

private:
	void boxRigidBodyAdded(entt::registry& reg, entt::entity ent);
	void boxRigidBodyRemoved(entt::registry& reg, entt::entity ent);

	void capsuleRigidBodyAdded(entt::registry& reg, entt::entity ent);
	void capsuleRigidBodyRemoved(entt::registry& reg, entt::entity ent);

	void coneRigidBodyAdded(entt::registry& reg, entt::entity ent);
	void coneRigidBodyRemoved(entt::registry& reg, entt::entity ent);

	void cylinderRigidBodyAdded(entt::registry& reg, entt::entity ent);
	void cylinderRigidBodyRemoved(entt::registry& reg, entt::entity ent);

	void sphereRigidBodyAdded(entt::registry& reg, entt::entity ent);
	void sphereRigidBodyRemoved(entt::registry& reg, entt::entity ent);

	void staticPlaneRigidBodyAdded(entt::registry& reg, entt::entity ent);
	void staticPlaneRigidBodyRemoved(entt::registry& reg, entt::entity ent);

	void triangleMeshRigidBodyAdded(entt::registry& reg, entt::entity ent);
	void triangleMeshRigidBodyRemoved(entt::registry& reg, entt::entity ent);

private:
	entt::registry registry_;
	entt::entity global_;
	bool physics_enabled_;
	std::unique_ptr<btDefaultCollisionConfiguration> bt_collision_configuration_;
	std::unique_ptr<btCollisionDispatcher>  bt_collision_dispatcher_;
	std::unique_ptr<btBroadphaseInterface> bt_overlapping_pair_cache_;
	std::unique_ptr<btSequentialImpulseConstraintSolver> bt_sequential_impulse_constraint_solver_;
	std::unique_ptr<btDynamicsWorld> bt_dynamics_world_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_H
