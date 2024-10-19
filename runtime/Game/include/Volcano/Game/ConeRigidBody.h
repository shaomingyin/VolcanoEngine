//
//
#ifndef VOLCANO_GAME_CONERIGIDBODY_H
#define VOLCANO_GAME_CONERIGIDBODY_H

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class ConeRigidBody: public btRigidBody {
public:
	ConeRigidBody(float mass, float radius, float height);
	~ConeRigidBody() override = default;

private:
	btConeShape shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONERIGIDBODY_H
