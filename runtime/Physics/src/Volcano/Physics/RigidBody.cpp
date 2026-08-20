//
//
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

RigidBody::RigidBody(QObject* parent)
    : Transformable(parent)
    , mass_(0)
    , shape_(nullptr) {
}

float RigidBody::mass() noexcept {
    return mass_;
}

void RigidBody::setMass(float v) noexcept {
    if (!qFuzzyCompare(mass_, v)) {
        mass_ = v;
        emit massChanged(mass_);
    }
}

Shape* RigidBody::shape() noexcept {
    return shape_;
}

void RigidBody::setShape(Shape* p) noexcept {
    if (shape_ != p) {
        shape_ = p;
        emit shapeChanged(shape_);
    }
}

VOLCANO_WORLD_END
