//
//
#include <Volcano/Physics/CapsuleRigidBody.h>

VOLCANO_PHYSICS_BEGIN

CapsuleRigidBody::CapsuleRigidBody(float mass, float radius, float height)
	: RigidBody(mass)
	, shape_(radius, height) {
	setCollisionShape(&shape_);
}

VOLCANO_PHYSICS_END
