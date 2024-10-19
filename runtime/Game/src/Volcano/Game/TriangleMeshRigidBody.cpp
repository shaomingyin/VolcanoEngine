//
//
#include <Volcano/Game/TriangleMeshRigidBody.h>

VOLCANO_GAME_BEGIN

TriangleMeshRigidBody::TriangleMeshRigidBody(float mass)
	: btRigidBody(mass, nullptr, nullptr)
	, shape_(nullptr, false) /* TODO */ {
	setCollisionShape(&shape_);
}

VOLCANO_GAME_END
