//
//
#ifndef VOLCANO_GAME_WORLD_H
#define VOLCANO_GAME_WORLD_H

#include <memory>
#include <type_traits>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Basic.h>
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

	virtual void frame(Duration elapsed);

	entt::handle create(std::string name = std::string()) {
		auto id = registry_.create();
		registry_.emplace<Basic>(id, std::move(name));
		return entt::handle(registry_, id);
	}

	template <typename Component>
	auto view() {
		return registry_.view<Component>();
	}

	template <typename Component, typename... Exclude>
	auto view(entt::exclude_t<Exclude...> exclude) {
		return registry_.view<Component>(std::forward<Exclude...>(exclude));
	}

	template <typename... Components>
	auto view() {
		return registry_.view<Components...>();
	}

	template <typename... Components, typename... Exclude>
	auto view(entt::exclude_t<Exclude...> exclude) {
		return registry_.view<Components...>(std::forward<Exclude...>(exclude));
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
	template <typename RigidBody>
	void rigidBodyAdded(entt::registry& registry, entt::entity entity) {
		static_assert(std::is_base_of<btRigidBody, RigidBody>::value);
		VOLCANO_ASSERT(&registry_ == &registry);
		auto& basic = registry.get<Basic>(entity);
		auto& rigid_body = registry.get<RigidBody>(entity);
		rigid_body.setMotionState(&basic);
		bt_dynamics_world_->addRigidBody(&rigid_body);
	}

	template <typename RigidBody>
	void rigidBodyRemoved(entt::registry& registry, entt::entity entity) {
		static_assert(std::is_base_of<btRigidBody, RigidBody>::value);
		VOLCANO_ASSERT(&registry_ == &registry);
		auto& rigid_body = registry.get<RigidBody>(entity);
		rigid_body.setMotionState(nullptr);
		bt_dynamics_world_->removeRigidBody(&rigid_body);
	}

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
