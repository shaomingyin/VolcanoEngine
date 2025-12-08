//
//
#include <Volcano/Game/RigidBody.h>

VOLCANO_GAME_BEGIN

RigidBody::MotionState::MotionState(Transformable &orig)
    : orig_(orig) {
}

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

QDataStream& operator<<(QDataStream& s, const RigidBody& v) {
    s << static_cast<const Transformable&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, RigidBody& v) {
    s >> static_cast<Transformable&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
