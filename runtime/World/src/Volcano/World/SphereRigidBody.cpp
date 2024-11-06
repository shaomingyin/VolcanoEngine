//
//
#include <Volcano/World/SphereRigidBody.h>

VOLCANO_WORLD_BEGIN

SphereRigidBody::SphereRigidBody(float mass, float radius)
	: RigidBody(mass)
	, shape_(radius) {
	setCollisionShape(&shape_);
}

VOLCANO_WORLD_END
