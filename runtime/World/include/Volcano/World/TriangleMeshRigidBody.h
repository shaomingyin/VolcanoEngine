//
//
#ifndef VOLCANO_WORLD_TRIANGLEMESHRIGIDBODY_H
#define VOLCANO_WORLD_TRIANGLEMESHRIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class TriangleMeshRigidBody: public RigidBody {
public:
	TriangleMeshRigidBody(float mass);
	~TriangleMeshRigidBody() override = default;

private:
	btBvhTriangleMeshShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRIANGLEMESHRIGIDBODY_H
