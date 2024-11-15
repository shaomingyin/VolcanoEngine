//
//
#include <Volcano/Physics/ConeRigidBody.h>

VOLCANO_PHYSICS_BEGIN

ConeRigidBody::ConeRigidBody(float mass, float radius, float height)
	: RigidBody(mass)
	, shape_(radius, height) {
	setCollisionShape(&shape_);
}

VOLCANO_PHYSICS_END
