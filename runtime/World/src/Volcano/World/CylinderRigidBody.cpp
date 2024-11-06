//
//
#include <Volcano/World/CylinderRigidBody.h>

VOLCANO_WORLD_BEGIN

CylinderRigidBody::CylinderRigidBody(float mass, Eigen::Vector3f h)
	: RigidBody(mass)
	, shape_(btVector3(h.x(), h.y(), h.z())) {
	setCollisionShape(&shape_);
}

VOLCANO_WORLD_END
