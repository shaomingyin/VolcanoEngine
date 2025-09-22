//
//
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

void RigidBody::MotionState::getWorldTransform(btTransform& world_transform) const {
    world_transform.setFromOpenGLMatrix(orig_.absoluteTransform().affine().data());
}

void RigidBody::MotionState::setWorldTransform(const btTransform& world_transform) {
    world_transform.getOpenGLMatrix(orig_.offset()->affine().data());
}

RigidBody::RigidBody(QObject* parent)
    : Transformable(parent)
    , motion_state_(*this)
    , rigid_body_(0.0f, &motion_state_, nullptr)
    , owner_world_(nullptr) {
}

RigidBody::~RigidBody() {
    if (owner_world_ != nullptr) {
        owner_world_->removeRigidBody(&rigid_body_);
    }
}

void RigidBody::addToWorld(btDynamicsWorld* p) {
    if (owner_world_ != p) {
        if (owner_world_ != nullptr) {
            owner_world_->removeRigidBody(&rigid_body_);
        }
        owner_world_ = p;
        if (owner_world_ != nullptr) {
            owner_world_->addRigidBody(&rigid_body_);
        }
    }
}

void RigidBody::classBegin() {
}

void RigidBody::componentComplete() {
}

VOLCANO_WORLD_END
