//
//
#ifndef VOLCANO_WORLD_CAPSULERIGIDBODY_H
#define VOLCANO_WORLD_CAPSULERIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class CapsuleRigidBody: public RigidBody {
public:
	CapsuleRigidBody(float mass, float radius, float height);
	~CapsuleRigidBody() override = default;

private:
	btCapsuleShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAPSULERIGIDBODY_H
