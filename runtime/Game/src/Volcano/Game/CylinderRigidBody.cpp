//
//
#include <Volcano/Game/CylinderRigidBody.h>

VOLCANO_GAME_BEGIN

CylinderRigidBody::CylinderRigidBody(float mass, Eigen::Vector3f h)
	: btRigidBody(mass, nullptr, nullptr)
	, shape_(btVector3(h.x(), h.y(), h.z())) {
	setCollisionShape(&shape_);
}

VOLCANO_GAME_END
