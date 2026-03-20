//
//
#include <Volcano/World/EntityPhysics.h>

VOLCANO_WORLD_BEGIN

EntityPhysics::EntityPhysics(QObject* parent)
    : QObject(parent) {
}

EntityPhysics::~EntityPhysics() {
}

void EntityPhysics::setBodyInterface(JPH::BodyInterface* body_interface) noexcept {
    if (body_id_.IsInvalid()) {
        Q_ASSERT(body_interface_ != nullptr);
        body_interface_->RemoveBody(body_id_);
        body_interface_->DestroyBody(body_id_);
        body_id_ = JPH::BodyID();
    }

    body_interface_ = body_interface;
    // body_id_ = bi->CreateBody()
}

void EntityPhysics::setEnabled(bool v) noexcept {
    if (enabled_ != v) {
        enabled_ = v;
        emit enabledChanged(v);
    }
}

void EntityPhysics::setCollisionShape(CollisionShape* p) noexcept {

}

VOLCANO_WORLD_END
