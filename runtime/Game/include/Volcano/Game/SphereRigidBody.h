//
//
#ifndef VOLCANO_GAME_SPHERERIGIDBODY_H
#define VOLCANO_GAME_SPHERERIGIDBODY_H

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class SphereRigidBody: public btRigidBody {
public:
	SphereRigidBody(float mass, float radius);
	~SphereRigidBody() override = default;

private:
	btSphereShape shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPHERERIGIDBODY_H
