//
//
#include <Volcano/Game/StaticPlaneRigidBody.h>

VOLCANO_GAME_BEGIN

StaticPlaneRigidBody::StaticPlaneRigidBody(float mass, Eigen::Vector3f normal, float constant)
	: btRigidBody(mass, nullptr, nullptr)
	, shape_(btVector3(normal.x(), normal.y(), normal.z()), constant) {
	setCollisionShape(&shape_);
}

VOLCANO_GAME_END
