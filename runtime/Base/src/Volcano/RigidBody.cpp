//
//
#include <Volcano/RigidBody.h>

VOLCANO_BEGIN

void RigidBody::MotionState::getWorldTransform(btTransform& world_transform) const {
    world_transform.setFromOpenGLMatrix(orig_.absoluteTransform().data());
}

void RigidBody::MotionState::setWorldTransform(const btTransform& world_transform) {
    world_transform.getOpenGLMatrix(orig_.transform().data());
}

RigidBody::RigidBody(QObject* parent)
    : Transformable(parent)
    , bt_rigid_body_(0.0f, nullptr, nullptr)
    , motion_state_(*this) {
    setMotionState(&motion_state_);
}

RigidBody::~RigidBody() {
    if (owner_world_ != nullptr) {
        owner_world_->removeRigidBody(this);
    }
}

VOLCANO_END
