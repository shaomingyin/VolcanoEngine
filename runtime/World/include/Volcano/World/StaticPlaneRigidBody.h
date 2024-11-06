//
//
#ifndef VOLCANO_WORLD_STATICPLANERIGIDBODY_H
#define VOLCANO_WORLD_STATICPLANERIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class StaticPlaneRigidBody: public RigidBody {
public:
	StaticPlaneRigidBody(float mass, Eigen::Vector3f normal, float constant);
	~StaticPlaneRigidBody() override = default;

private:
	btStaticPlaneShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_STATICPLANERIGIDBODY_H
