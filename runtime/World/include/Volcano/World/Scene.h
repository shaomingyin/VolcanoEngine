//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <type_traits>

#include <taskflow/taskflow.hpp>

#include <Volcano/World/Common.h>
#include <Volcano/World/Basic.h>
#include <Volcano/World/Light.h>
#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

class Scene {
public:
	Scene();
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	virtual ~Scene();

public:
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;

	tf::Executor& executor() {
		return executor_;
	}

	entt::handle createEntity(std::string name = std::string()) {
		auto id = registry_.create();
		registry_.emplace<Basic>(id, std::move(name));
		return entt::handle(registry_, id);
	}

	template <typename Component>
	auto componentView() {
		return registry_.view<Component>();
	}

	template <typename Component, typename... Exclude>
	auto componentView(entt::exclude_t<Exclude...> exclude) {
		return registry_.view<Component>(std::forward<Exclude...>(exclude));
	}

	template <typename... Components>
	auto componentView() {
		return registry_.view<Components...>();
	}

	template <typename... Components, typename... Exclude>
	auto componentView(entt::exclude_t<Exclude...> exclude) {
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

	void drawPhysicsDebug() {
		bt_dynamics_world_->debugDrawWorld();
	}

	btIDebugDraw* physicsDebugDrawer() {
		return bt_dynamics_world_->getDebugDrawer();
	}

	const btIDebugDraw* physicsDebugDrawer() const {
		return bt_dynamics_world_->getDebugDrawer();
	}

	void setPhysicsDebugDrawer(btIDebugDraw* p) {
		bt_dynamics_world_->setDebugDrawer(p);
	}

protected:
	entt::registry& registry() {
		return registry_;
	}

	const entt::registry& registry() const {
		return registry_;
	}

	virtual void frame(Duration elapsed);

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
	tf::Executor executor_;
	entt::registry registry_;
	entt::entity global_;
	bool physics_enabled_;
	std::unique_ptr<btDefaultCollisionConfiguration> bt_collision_configuration_;
	std::unique_ptr<btCollisionDispatcher>  bt_collision_dispatcher_;
	std::unique_ptr<btBroadphaseInterface> bt_overlapping_pair_cache_;
	std::unique_ptr<btSequentialImpulseConstraintSolver> bt_sequential_impulse_constraint_solver_;
	std::unique_ptr<btDynamicsWorld> bt_dynamics_world_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
