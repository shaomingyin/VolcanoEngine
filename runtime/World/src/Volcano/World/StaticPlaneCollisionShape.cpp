//
//
#include <Volcano/World/StaticPlaneCollisionShape.h>

VOLCANO_WORLD_BEGIN

StaticPlaneCollisionShape::StaticPlaneCollisionShape(QObject* parent)
    : CollisionShape(parent)
    , normal_(0.0f, 1.0f, 0.0f)
    , constant_(0.0f) {
}

void StaticPlaneCollisionShape::setNormal(const QVector3D& v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'normal' property after component completed.";
        return;
    }
    if (normal_ != v) {
        normal_ =  v;
        emit normalChanged(v);
    }
}

void StaticPlaneCollisionShape::setConstant(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'constant' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(constant_, v)) {
        constant_ =  v;
        emit constantChanged(v);
    }
}

void StaticPlaneCollisionShape::componentComplete() {
    //setFlags()STATIC;
    shape_ = std::make_unique<btStaticPlaneShape>(
        btVector3(normal_.x(), normal_.y(), normal_.z()), constant_);
    setCollisionShape(shape_.get());
}

VOLCANO_WORLD_END
