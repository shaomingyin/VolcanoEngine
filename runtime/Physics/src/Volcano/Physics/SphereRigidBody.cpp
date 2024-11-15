//
//
#include <Volcano/Physics/SphereRigidBody.h>

VOLCANO_PHYSICS_BEGIN

SphereRigidBody::SphereRigidBody(float mass, float radius)
	: RigidBody(mass)
	, shape_(radius) {
	setCollisionShape(&shape_);
}

VOLCANO_PHYSICS_END
