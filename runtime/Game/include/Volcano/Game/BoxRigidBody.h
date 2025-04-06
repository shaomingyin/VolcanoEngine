//
//
#ifndef VOLCANO_GAME_BOXRIGIDBODY_H
#define VOLCANO_GAME_BOXRIGIDBODY_H

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class BoxRigidBody: public btRigidBody {
public:
	BoxRigidBody(float mass, Eigen::Vector3f h);
	~BoxRigidBody() override = default;

private:
	btBoxShape shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_BOXRIGIDBODY_H
