//
//
#ifndef VOLCANO_WORLD_RIGIDBODY_H
#define VOLCANO_WORLD_RIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class RigidBody: public Transformable, public btRigidBody {
public:
	RigidBody(float mass);
	virtual ~RigidBody() = default;

public:

private:
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_RIGIDBODY_H
