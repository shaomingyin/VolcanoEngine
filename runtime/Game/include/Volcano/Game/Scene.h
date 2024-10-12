//
//
#ifndef VOLCANO_GAME_SCENE_H
#define VOLCANO_GAME_SCENE_H

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class Scene: public Object {
public:
	Scene(Context& context);
	~Scene() override;

public:
	bool isPhysicsEnabled() {
		return bool(bt_dynamics_world_);
	}

	void enablePhysics();
	void disablePhysics();

	Eigen::Vector3f gravity() const {
		return gravity_;
	}

	void setGravity(Eigen::Vector3f v);

private:
	Eigen::Vector3f gravity_;
	std::unique_ptr<btDynamicsWorld> bt_dynamics_world_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_H
