//
//
#ifndef VOLCANO_WORLD_SPHERERIGIDBODY_H
#define VOLCANO_WORLD_SPHERERIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class SphereRigidBody: public RigidBody {
public:
	SphereRigidBody(float mass, float radius);
	~SphereRigidBody() override = default;

private:
	btSphereShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SPHERERIGIDBODY_H
