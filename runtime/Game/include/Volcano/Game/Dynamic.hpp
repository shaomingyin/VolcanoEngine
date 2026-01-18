//
//
#ifndef VOLCANO_GAME_DYNAMIC_HPP
#define VOLCANO_GAME_DYNAMIC_HPP

#include <btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Dynamic {
public:
	Dynamic();
	virtual ~Dynamic();

public:
	bool isPhysicsEnabled() const noexcept {
		return physics_enabled_;
	}

	void enablePhysics() noexcept {
		physics_enabled_ = true;
	}

	void disablePhysics() noexcept {
		physics_enabled_ = false;
	}

	const Eigen::Vector3f& getGravity() const noexcept {
		return gravity_;
	}

	void setGravity(const Eigen::Vector3f& v) noexcept;

	btDynamicsWorld& world() noexcept {
		return bt_world_;
	}

	const btDynamicsWorld& world() const noexcept {
		return bt_world_;
	}

	void update(Clock::duration elapsed);

private:
	bool physics_enabled_;
	Eigen::Vector3f gravity_;
	btDefaultCollisionConfiguration bt_config_;
	btCollisionDispatcher bt_dispatcher_;
	btDbvtBroadphase bt_broadphase_;
	btSequentialImpulseConstraintSolver bt_solver_;
	btDiscreteDynamicsWorld bt_world_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DYNAMIC_HPP
