//
//
#include <Volcano/World/BoxRigidBody.h>

VOLCANO_WORLD_BEGIN

BoxRigidBody::BoxRigidBody(QObject* parent)
    : RigidBody(parent)
    , size_(1.0f, 1.0f, 1.0f) {
}

void BoxRigidBody::setWidth(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'width' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(size_.z(), v)) {
        size_.setZ(v);
        emit widthChanged(v);
    }
}

void BoxRigidBody::setHeight(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'height' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(size_.y(), v)) {
        size_.setY(v);
        emit heightChanged(v);
    }
}

void BoxRigidBody::componentComplete() {
    shape_ = std::make_unique<btBoxShape>(
        btVector3{ size_.x() / 2.0f, size_.y() / 2.0f, size_.z() / 2.0f });
    setCollisionShape(shape_.get());
}

QDataStream& operator<<(QDataStream& s, const BoxRigidBody& v) {
    s << static_cast<const RigidBody&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, BoxRigidBody& v) {
    s >> static_cast<RigidBody&>(v);
    // TODO
    return s;
}

VOLCANO_WORLD_END
