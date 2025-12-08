//
//
#include <Volcano/Game/CylinderRigidBody.h>

VOLCANO_GAME_BEGIN

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

QDataStream& operator<<(QDataStream& s, const CylinderRigidBody& v) {
    s << static_cast<const RigidBody&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, CylinderRigidBody& v) {
    s >> static_cast<RigidBody&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
