//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Inherency.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
public:
	Scene();
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	virtual ~Scene();

public:
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;

	entt::handle global() const {
		return global_;
	}

	entt::handle createEntity() {
		return entt::handle(*this, create());
	}

	bool isPhysicsEnabled() const {
		return bool(bt_dynamics_world_);
	}

	const Eigen::Vector3f& gravity() const {
		return gravity_;
	}

	void setGravity(const Eigen::Vector3f& v) {
		gravity_ = v;
		if (bt_dynamics_world_) {
			bt_dynamics_world_->setGravity(btVector3(v.x(), v.y(), v.x()));
		}
	}

	void enablePhysics();
	void disablePhysics();
	virtual void update(Duration elapsed);

private:
	void rigidBodyAdded(RigidBody& body, Inherency* inherency = nullptr);
	void rigidBodyRemoved(RigidBody& body);

	template <typename T>
	void onRigidBodyAdded(entt::registry& registry, entt::entity entity) {

	}

	template <typename T>
	void onRigidBodyRemoved(entt::registry& registry, entt::entity entity) {
	}

private:
	entt::handle global_;
	Eigen::Vector3f gravity_;
	std::unique_ptr<btDefaultCollisionConfiguration> bt_collision_configuration_;
	std::unique_ptr<btCollisionDispatcher>  bt_collision_dispatcher_;
	std::unique_ptr<btBroadphaseInterface> bt_overlapping_pair_cache_;
	std::unique_ptr<btSequentialImpulseConstraintSolver> bt_sequential_impulse_constraint_solver_;
	std::unique_ptr<btDynamicsWorld> bt_dynamics_world_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
