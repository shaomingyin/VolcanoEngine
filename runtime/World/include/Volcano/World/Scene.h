//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <type_traits>

#include <Volcano/World/Common.h>
#include <Volcano/World/Entity.h>
#include <Volcano/World/RigidBody.h>

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

	Entity global() {
		return global_;
	}

	Entity emplaceEntity() {
		return Entity(registry_, registry_.create());
	}

	Entity emplaceEntity(std::string name) {
		return Entity(registry_, registry_.create(), std::move(name), Eigen::Affine3f::Identity());
	}

	Entity emplaceEntity(std::string name, const Eigen::Affine3f& transform) {
		return Entity(registry_, registry_.create(), std::move(name), transform);
	}

	template <typename Component, typename... Args>
	Component& emplaceGlobalComponent(Args... args) {
		return global_.emplace<Component>(std::forward<Args>(args)...);
	}

	template <typename... Components>
	void removeGlobalComponents() {
		return global_.erase<Components...>();
	}

	template <typename Component>
	[[nodiscard]] auto componentView() {
		return registry_.view<Component>();
	}

	template <typename Component, typename... Exclude>
	[[nodiscard]] auto componentView(entt::exclude_t<Exclude...> exclude) {
		return registry_.view<Component>(std::forward<Exclude...>(exclude));
	}

	template <typename... Components>
	[[nodiscard]] auto componentView() {
		return registry_.view<Components...>();
	}

	template <typename... Components, typename... Exclude>
	[[nodiscard]] auto componentView(entt::exclude_t<Exclude...> exclude) {
		return registry_.view<Components...>(std::forward<Exclude...>(exclude));
	}

	template <typename Component>
	[[nodiscard]] auto componentAddedSignal(Entity ent = Entity()) {
		return registry_.on_construct<Component>(ent.handle_);
	}

	template <typename Component>
	[[nodiscard]] auto globalComponentAddedSignal() {
		return componentAddedSignal(global_.handle_);
	}

	template <typename Component, typename Candidate, typename... Type>
	[[nodiscard]] auto onComponentAdded(Entity ent, Type&&... value_or_instance) {
		return componentAddedSignal<Component>(ent).connect<Candidate>(std::forward<Type>(value_or_instance)...);
	}

	template <typename Component, typename Candidate, typename... Type>
	[[nodiscard]] auto onComponentAdded(Type&&... value_or_instance) {
		return onComponentAdded<Component>(Entity(), std::forward<Type>(value_or_instance)...);
	}

	template <typename Component, typename Candidate, typename... Type>
	[[nodiscard]] auto onGlobalComponentAdded(Type&&... value_or_instance) {
		return onComponentAdded<Component>(global_, std::forward<Type>(value_or_instance)...);
	}

	template <typename Component>
	[[nodiscard]] auto componentRemovedSignal(Entity ent = Entity()) {
		return registry_.on_destroy<Component>(ent.handle_);
	}

	template <typename Component, typename Candidate, typename... Type>
	[[nodiscard]] auto onComponentRemoved(Entity ent, Type&&... value_or_instance) {
		return componentRemovedSignal<Component>(ent).connect<Candidate>(std::forward<Type>(value_or_instance)...);
	}

	template <typename Component, typename Candidate, typename... Type>
	[[nodiscard]] auto onComponentRemoved(Type&&... value_or_instance) {
		return onComponentRemoved(Entity(), std::forward<Type>(value_or_instance)...);
	}

	template <typename Component, typename Candidate, typename... Type>
	[[nodiscard]] auto onGlobalComponentRemoved(Type&&... value_or_instance) {
		return onComponentRemoved(global_, std::forward<Type>(value_or_instance)...);
	}

	template <typename Component>
	[[nodiscard]] auto componentUpdatedSignal(Entity ent = Entity()) {
		return registry_.on_update<Component>(ent.handle_);
	}

	template <typename Component, typename Candidate, typename... Type>
	[[nodiscard]] auto onComponentUpdated(Entity ent, Type... value_or_instance) {
		return componentUpdatedSignal<Component>(ent.handle_).connect<Candidate>(std::forward<Type>(value_or_instance)...);
	}

	template <typename Component, typename Candidate, typename... Type>
	[[nodiscard]] auto onComponentUpdated(Type... value_or_instance) {
		return onComponentUpdated(Entity(), std::forward<Type>(value_or_instance)...);
	}

	template <typename Component, typename Candidate, typename... Type>
	[[nodiscard]] auto onGlobalComponentUpdated(Type... value_or_instance) {
		return onComponentUpdated(global_, std::forward<Type>(value_or_instance)...);
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

	void setGravity(const Eigen::Vector3f& v) {
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
	template <typename T>
	void rigidBodyAdded(entt::registry& registry, entt::entity entity) {
		VOLCANO_ASSERT(&registry_ == &registry);
		static_assert(std::is_base_of_v<RigidBody, T>);
		Entity ent(registry, entity);
		if (ent.isValid()) {
			auto& rigid_body = registry.get<T>(entity);
			rigid_body.setTransform(ent.transform());
			bt_dynamics_world_->addRigidBody(&rigid_body);
		}
	}

	template <typename T>
	void rigidBodyRemoved(entt::registry& registry, entt::entity entity) {
		VOLCANO_ASSERT(&registry_ == &registry);
		static_assert(std::is_base_of_v<RigidBody, T>);
		auto& rigid_body = registry.get<T>(entity);
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
