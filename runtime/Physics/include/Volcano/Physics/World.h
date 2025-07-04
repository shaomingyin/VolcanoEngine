//
//
#ifndef VOLCANO_PHYSICS_WORLD_H
#define VOLCANO_PHYSICS_WORLD_H

#include <btBulletDynamicsCommon.h>

#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

class World {
public:
	World();
	virtual ~World() = default;

public:
	bool isEnabled() const noexcept {
		return enabled_;
	}

	void enable() noexcept {
		enabled_ = true;
	}

	void disable() noexcept {
		enabled_ = false;
	}

	const Eigen::Vector3f& gravity() const noexcept {
		return gravity_;
	}	

	void setGravity(const Eigen::Vector3f& v);
	void update(Duration elapsed);
	
private:
	bool enabled_;
	Eigen::Vector3f gravity_;
	btDefaultCollisionConfiguration bt_collision_configuration_;
	btCollisionDispatcher bt_collsion_dispatcher_;
	btDbvtBroadphase bt_broadphase_interface_;
	btSequentialImpulseConstraintSolver bt_constraint_solver_;
	btDiscreteDynamicsWorld bt_dynamic_world_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_WORLD_H
