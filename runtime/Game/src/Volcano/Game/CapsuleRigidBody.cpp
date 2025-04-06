//
//
#include <Volcano/Game/CapsuleRigidBody.h>

VOLCANO_GAME_BEGIN

CapsuleRigidBody::CapsuleRigidBody(float mass, float radius, float height)
	: btRigidBody(mass, nullptr, nullptr)
	, shape_(radius, height) {
	setCollisionShape(&shape_);
}

VOLCANO_GAME_END
