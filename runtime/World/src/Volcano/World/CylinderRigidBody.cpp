//
//
#include <Volcano/World/CylinderRigidBody.h>

VOLCANO_WORLD_BEGIN

CylinderRigidBody::CylinderRigidBody(QObject* parent)
    : RigidBody(parent)
    , size_(1.0f, 1.0f, 1.0f) {
}

void CylinderRigidBody::resize(const Vector3& v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'size' property after component completed.";
        return;
    }
    if (size_ != v) {
        size_ = v;
        emit sizeChanged(v);
    }
}

void CylinderRigidBody::componentComplete() {
    shape_ = std::make_unique<btCylinderShape>(
        btVector3(size_.x(), size_.y(), size_.z()));
    setCollisionShape(shape_.get());
}


VOLCANO_WORLD_END
