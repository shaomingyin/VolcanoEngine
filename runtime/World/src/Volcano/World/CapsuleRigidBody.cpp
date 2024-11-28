//
//
#include <Volcano/World/CapsuleRigidBody.h>

VOLCANO_WORLD_BEGIN

CapsuleRigidBody::CapsuleRigidBody(float mass, float radius, float height)
	: RigidBody(mass)
	, shape_(radius, height) {
	setCollisionShape(&shape_);
}

VOLCANO_WORLD_END
