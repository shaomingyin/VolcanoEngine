//
//
#include <Volcano/Game/SphereRigidBody.h>

VOLCANO_GAME_BEGIN

SphereRigidBody::SphereRigidBody(float mass, float radius)
	: btRigidBody(mass, nullptr, nullptr)
	, shape_(radius) {
	setCollisionShape(&shape_);
}

VOLCANO_GAME_END
