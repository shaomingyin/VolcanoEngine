//
//
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

CollisionShape::CollisionShape(QObject* parent)
    : QObject(parent)
    , shape_(nullptr) {
}

CollisionShape::~CollisionShape() {
}

float CollisionShape::mass() const noexcept {
    Q_ASSERT(shape_ != nullptr);
    return shape_->GetMassProperties().mMass;
}

void CollisionShape::setMass(float v) noexcept {
    Q_ASSERT(shape_ != nullptr);
    auto mp = shape_->GetMassProperties();
    if (mp.mMass != v) {
        mp.mMass = v;
        emit massChanged(v);
    }
}

VOLCANO_WORLD_END
