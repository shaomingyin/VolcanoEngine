//
//
#ifndef VOLCANO_WORLD_CYLINDERRIGIDBODY_H
#define VOLCANO_WORLD_CYLINDERRIGIDBODY_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class CylinderRigidBody: public btRigidBody {
public:
	CylinderRigidBody(float mass, Eigen::Vector3f h);
	~CylinderRigidBody() override = default;

private:
	btCylinderShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CYLINDERRIGIDBODY_H
