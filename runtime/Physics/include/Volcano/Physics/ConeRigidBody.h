//
//
#ifndef VOLCANO_PHYSICS_CONERIGIDBODY_H
#define VOLCANO_PHYSICS_CONERIGIDBODY_H

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class ConeRigidBody: public RigidBody {
public:
	ConeRigidBody(float mass, float radius, float height);
	~ConeRigidBody() override = default;

private:
	btConeShape shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_CONERIGIDBODY_H
