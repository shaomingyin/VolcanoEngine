//
//
#include <Volcano/Physics/TriangleMeshRigidBody.h>

VOLCANO_PHYSICS_BEGIN

TriangleMeshRigidBody::TriangleMeshRigidBody(float mass)
	: RigidBody(mass)
	, shape_(nullptr, false) /* TODO */ {
	setCollisionShape(&shape_);
}

VOLCANO_PHYSICS_END
