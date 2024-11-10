//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <type_traits>

#include <Volcano/World/Common.h>
#include <Volcano/World/Entity.h>
#include <Volcano/World/Component.h>

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

	virtual void frame(Duration elapsed);

	Entity emplaceEntity() {
		return Entity(registry_, registry_.create());
	}

	Entity emplaceEntity(std::string name) {
		return Entity(registry_, registry_.create(), std::move(name), Eigen::Affine3f::Identity());
	}

	Entity emplaceEntity(std::string name, const Eigen::Affine3f& transform) {
		return Entity(registry_, registry_.create(), std::move(name), transform);
	}

	Entity global() {
		return global_;
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

private:
	template <RigidBodyType RigidBody>
	void rigidBodyAdded(entt::registry& registry, entt::entity entity) {
		VOLCANO_ASSERT(&registry_ == &registry);
		Entity ent(registry, entity);
		if (ent.isValid()) {
			auto& rigid_body = registry.get<RigidBody>(entity);
			rigid_body.setTransform(ent.transform());
			bt_dynamics_world_->addRigidBody(&rigid_body);
		}
	}

	template <RigidBodyType RigidBody>
	void rigidBodyRemoved(entt::registry& registry, entt::entity entity) {
		VOLCANO_ASSERT(&registry_ == &registry);
		auto& rigid_body = registry.get<RigidBody>(entity);
		rigid_body.resetTransform();
		bt_dynamics_world_->removeRigidBody(&rigid_body);
	}

private:
	entt::registry registry_;
	Entity global_;
	bool physics_enabled_;
	std::unique_ptr<btDefaultCollisionConfiguration> bt_collision_configuration_;
	std::unique_ptr<btCollisionDispatcher>  bt_collision_dispatcher_;
	std::unique_ptr<btBroadphaseInterface> bt_overlapping_pair_cache_;
	std::unique_ptr<btSequentialImpulseConstraintSolver> bt_sequential_impulse_constraint_solver_;
	std::unique_ptr<btDynamicsWorld> bt_dynamics_world_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
