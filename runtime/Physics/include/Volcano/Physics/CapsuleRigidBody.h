//
//
#ifndef VOLCANO_PHYSICS_CAPSULERIGIDBODY_H
#define VOLCANO_PHYSICS_CAPSULERIGIDBODY_H

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class CapsuleRigidBody: public RigidBody {
public:
	CapsuleRigidBody(float mass, float radius, float height);
	~CapsuleRigidBody() override = default;

private:
	btCapsuleShape shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_CAPSULERIGIDBODY_H
