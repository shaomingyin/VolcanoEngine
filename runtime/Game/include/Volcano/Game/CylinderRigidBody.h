//
//
#ifndef VOLCANO_GAME_CYLINDERRIGIDBODY_H
#define VOLCANO_GAME_CYLINDERRIGIDBODY_H

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class CylinderRigidBody: public btRigidBody {
public:
	CylinderRigidBody(float mass, Eigen::Vector3f h);
	~CylinderRigidBody() override = default;

private:
	btCylinderShape shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CYLINDERRIGIDBODY_H
