//
//
#ifndef VOLCANO_WORLD_CONERIGIDBODY_H
#define VOLCANO_WORLD_CONERIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class ConeRigidBody: public RigidBody {
public:
	ConeRigidBody(float mass, float radius, float height);
	~ConeRigidBody() override = default;

private:
	btConeShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONERIGIDBODY_H
