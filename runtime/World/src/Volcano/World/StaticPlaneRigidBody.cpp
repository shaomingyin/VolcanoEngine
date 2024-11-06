//
//
#include <Volcano/World/StaticPlaneRigidBody.h>

VOLCANO_WORLD_BEGIN

StaticPlaneRigidBody::StaticPlaneRigidBody(float mass, Eigen::Vector3f normal, float constant)
	: RigidBody(mass)
	, shape_(btVector3(normal.x(), normal.y(), normal.z()), constant) {
	setCollisionShape(&shape_);
	// setFlags(getFlags() | ) STATIC OBJECT?
}

VOLCANO_WORLD_END
