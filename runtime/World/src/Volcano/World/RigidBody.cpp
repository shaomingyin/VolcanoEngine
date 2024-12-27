//
//
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

void RigidBody::MotionState::getWorldTransform(btTransform& world_transform) const {
    world_transform.setFromOpenGLMatrix(orig_.absoluteTransform().data());
}

void RigidBody::MotionState::setWorldTransform(const btTransform& world_transform) {
    world_transform.getOpenGLMatrix(orig_.transform()->data());
}

RigidBody::RigidBody(QObject* parent)
    : Transformable(parent)
    , motion_state_(*this)
    , rigid_body_(0.0f, &motion_state_, nullptr) {
}

RigidBody::~RigidBody() {
    if (owner_world_ != nullptr) {
        owner_world_->removeRigidBody(&rigid_body_);
    }
}

VOLCANO_WORLD_END
