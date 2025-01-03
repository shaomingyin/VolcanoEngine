//
//
#include <Volcano/World/StaticPlaneRigidBody.h>

VOLCANO_WORLD_BEGIN

StaticPlaneRigidBody::StaticPlaneRigidBody(QObject* parent)
    : RigidBody(parent)
    , normal_(0.0f, 1.0f, 0.0f)
    , constant_(0.0f) {
}

void StaticPlaneRigidBody::setNormal(const Vector3& v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'normal' property after component completed.";
        return;
    }
    if (normal_ != v) {
        normal_ =  v;
        emit normalChanged(v);
    }
}

void StaticPlaneRigidBody::setConstant(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'constant' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(constant_, v)) {
        constant_ =  v;
        emit constantChanged(v);
    }
}

void StaticPlaneRigidBody::componentComplete() {
    //setFlags()STATIC;
    shape_ = std::make_unique<btStaticPlaneShape>(
        btVector3(normal_.x(), normal_.y(), normal_.z()), constant_);
    setCollisionShape(shape_.get());
}

VOLCANO_WORLD_END
