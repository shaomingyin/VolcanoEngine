//
//
#ifndef VOLCANO_PHYSICS_CYLINDERRIGIDBODY_H
#define VOLCANO_PHYSICS_CYLINDERRIGIDBODY_H

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class CylinderRigidBody: public RigidBody {
public:
	CylinderRigidBody(float mass, Eigen::Vector3f h);
	~CylinderRigidBody() override = default;

private:
	btCylinderShape shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_CYLINDERRIGIDBODY_H
