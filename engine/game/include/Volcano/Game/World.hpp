//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class World {
	VOLCANO_DISABLE_COPY_AND_MOVE(World)

public:
	World(void);
	virtual ~World(void);

public:
	virtual bool init(void);
	virtual void update(float elapsed);

	const Eigen::Vector3f &gravity(void) const;
	void setGravity(const Eigen::Vector3f &v);

public:
	sigslot::signal<const Eigen::Vector3f> onGravityChanged;

private:
	entt::registry m_ecs;
	Eigen::Vector3f m_gravity;
	btDynamicsWorld *m_btWorld;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
