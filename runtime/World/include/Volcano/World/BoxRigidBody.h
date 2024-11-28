//
//
#ifndef VOLCANO_WORLD_BOXRIGIDBODY_H
#define VOLCANO_WORLD_BOXRIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class BoxRigidBody: public RigidBody {
public:
	BoxRigidBody(float mass, const Eigen::Vector3f& h);
	~BoxRigidBody() override = default;

private:
	btBoxShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BOXRIGIDBODY_H
