//
//
#ifndef VOLCANO_PHYSICS_TRIANGLEMESHRIGIDBODY_H
#define VOLCANO_PHYSICS_TRIANGLEMESHRIGIDBODY_H

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class TriangleMeshRigidBody: public RigidBody {
public:
	TriangleMeshRigidBody(float mass);
	~TriangleMeshRigidBody() override = default;

private:
	btBvhTriangleMeshShape shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_TRIANGLEMESHRIGIDBODY_H
