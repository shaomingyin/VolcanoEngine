//
//
#ifndef VOLCANO_WORLD_BOXRIGIDBODY_H
#define VOLCANO_WORLD_BOXRIGIDBODY_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class BoxRigidBody: public btRigidBody {
public:
	BoxRigidBody(float mass, Eigen::Vector3f h);
	~BoxRigidBody() override = default;

private:
	btBoxShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BOXRIGIDBODY_H
