//
//
#include <Volcano/World/ConeRigidBody.h>

VOLCANO_WORLD_BEGIN

ConeRigidBody::ConeRigidBody(float mass, float radius, float height)
	: RigidBody(mass)
	, shape_(radius, height) {
	setCollisionShape(&shape_);
}

VOLCANO_WORLD_END
