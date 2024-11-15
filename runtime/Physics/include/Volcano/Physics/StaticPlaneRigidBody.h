//
//
#ifndef VOLCANO_PHYSICS_STATICPLANERIGIDBODY_H
#define VOLCANO_PHYSICS_STATICPLANERIGIDBODY_H

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class StaticPlaneRigidBody: public RigidBody {
public:
	StaticPlaneRigidBody(float mass, Eigen::Vector3f normal, float constant);
	~StaticPlaneRigidBody() override = default;

private:
	btStaticPlaneShape shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_STATICPLANERIGIDBODY_H
