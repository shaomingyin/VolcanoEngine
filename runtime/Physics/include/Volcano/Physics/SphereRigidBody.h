//
//
#ifndef VOLCANO_PHYSICS_SPHERERIGIDBODY_H
#define VOLCANO_PHYSICS_SPHERERIGIDBODY_H

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class SphereRigidBody: public RigidBody {
public:
	SphereRigidBody(float mass, float radius);
	~SphereRigidBody() override = default;

private:
	btSphereShape shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_SPHERERIGIDBODY_H
