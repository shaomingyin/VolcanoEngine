//
//
#ifndef VOLCANO_WORLD_RIGIDBODY_H
#define VOLCANO_WORLD_RIGIDBODY_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class RigidBody {
public:
	RigidBody(float mass = 0.0f);
	virtual ~RigidBody();

public:

private:
	//btRigidBody bt_rigid_body_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_RIGIDBODY_H
