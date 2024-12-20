//
//
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

void RigidBody::MotionState::getWorldTransform(btTransform& world_transform) const {
    world_transform.setFromOpenGLMatrix(orig_.worldTransform().data());
}

void RigidBody::MotionState::setWorldTransform(const btTransform& world_transform) {
    QMatrix4x4 tmp;
    world_transform.getOpenGLMatrix(tmp.data());
    orig_.setTransform(tmp);
}

RigidBody::RigidBody(QObject* parent)
    : World::Transformable(parent)
    , btRigidBody(0.0f, nullptr, nullptr)
    , motion_state_(*this) {
    setMotionState(&motion_state_);
}

RigidBody::~RigidBody() {
    if (owner_world_ != nullptr) {
        owner_world_->removeRigidBody(this);
    }
}

VOLCANO_PHYSICS_END
