//
//
#ifndef VOLCANO_WORLD_CYLINDERRIGIDBODY_H
#define VOLCANO_WORLD_CYLINDERRIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class CylinderRigidBody: public RigidBody {
public:
	CylinderRigidBody(float mass, Eigen::Vector3f h);
	~CylinderRigidBody() override = default;

private:
	btCylinderShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CYLINDERRIGIDBODY_H
