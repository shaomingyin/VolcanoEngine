//
//
#ifndef VOLCANO_GAME_TRIANGLEMESHRIGIDBODY_H
#define VOLCANO_GAME_TRIANGLEMESHRIGIDBODY_H

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class TriangleMeshRigidBody: public btRigidBody {
public:
	TriangleMeshRigidBody(float mass);
	~TriangleMeshRigidBody() override = default;

private:
	btBvhTriangleMeshShape shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_TRIANGLEMESHRIGIDBODY_H
