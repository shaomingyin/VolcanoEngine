//
//
#include <Volcano/Physics/CylinderRigidBody.h>

VOLCANO_PHYSICS_BEGIN

CylinderRigidBody::CylinderRigidBody(float mass, Eigen::Vector3f h)
	: RigidBody(mass)
	, shape_(btVector3(h.x(), h.y(), h.z())) {
	setCollisionShape(&shape_);
}

VOLCANO_PHYSICS_END
