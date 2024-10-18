//
//
#ifndef VOLCANO_GAME_RIGIDBODY_H
#define VOLCANO_GAME_RIGIDBODY_H

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class RigidBody {
public:
	RigidBody(float mass = 0.0f);
	virtual ~RigidBody();

public:

private:
	//btRigidBody bt_rigid_body_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_RIGIDBODY_H
