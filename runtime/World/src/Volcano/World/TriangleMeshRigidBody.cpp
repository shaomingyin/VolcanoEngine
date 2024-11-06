//
//
#include <Volcano/World/TriangleMeshRigidBody.h>

VOLCANO_WORLD_BEGIN

TriangleMeshRigidBody::TriangleMeshRigidBody(float mass)
	: RigidBody(mass)
	, shape_(nullptr, false) /* TODO */ {
	setCollisionShape(&shape_);
}

VOLCANO_WORLD_END
