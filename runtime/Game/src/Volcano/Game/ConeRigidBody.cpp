//
//
#include <Volcano/Game/ConeRigidBody.h>

VOLCANO_GAME_BEGIN

ConeRigidBody::ConeRigidBody(float mass, float radius, float height)
	: btRigidBody(mass, nullptr, nullptr)
	, shape_(radius, height) {
	setCollisionShape(&shape_);
}

VOLCANO_GAME_END
