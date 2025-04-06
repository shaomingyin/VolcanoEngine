//
//
#ifndef VOLCANO_GAME_CAPSULERIGIDBODY_H
#define VOLCANO_GAME_CAPSULERIGIDBODY_H

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class CapsuleRigidBody: public btRigidBody {
public:
	CapsuleRigidBody(float mass, float radius, float height);
	~CapsuleRigidBody() override = default;

private:
	btCapsuleShape shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CAPSULERIGIDBODY_H
