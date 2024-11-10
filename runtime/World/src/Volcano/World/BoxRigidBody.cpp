//
//
#include <Volcano/World/BoxRigidBody.h>

VOLCANO_WORLD_BEGIN

BoxRigidBody::BoxRigidBody(float mass, const Eigen::Vector3f& h)
	: RigidBody(mass)
	, offset_(Eigen::Affine3f::Identity())
	, shape_(btVector3(h.x(), h.y(), h.z())) {
	setCollisionShape(&shape_);
}

VOLCANO_WORLD_END
