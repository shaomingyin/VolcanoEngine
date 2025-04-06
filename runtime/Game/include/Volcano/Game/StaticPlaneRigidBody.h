//
//
#ifndef VOLCANO_GAME_STATICPLANERIGIDBODY_H
#define VOLCANO_GAME_STATICPLANERIGIDBODY_H

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class StaticPlaneRigidBody: public btRigidBody {
public:
	StaticPlaneRigidBody(float mass, Eigen::Vector3f normal, float constant);
	~StaticPlaneRigidBody() override = default;

private:
	btStaticPlaneShape shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_STATICPLANERIGIDBODY_H
