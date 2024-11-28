//
//
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

RigidBody::RigidBody(float mass)
    : btRigidBody(mass, nullptr, nullptr) {
}

VOLCANO_WORLD_END
