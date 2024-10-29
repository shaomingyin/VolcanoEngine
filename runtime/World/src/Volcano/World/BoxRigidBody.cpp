//
//
#include <Volcano/World/BoxRigidBody.h>

VOLCANO_WORLD_BEGIN

BoxRigidBody::BoxRigidBody(float mass, Eigen::Vector3f h)
	: btRigidBody(mass, nullptr, nullptr)
	, shape_(btVector3(h.x(), h.y(), h.z())) {
	setCollisionShape(&shape_);
}

VOLCANO_WORLD_END
