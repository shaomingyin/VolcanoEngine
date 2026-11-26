//
//
#include <stdexcept>

#include <Volcano/Physics/CollisionShape.h>

VOLCANO_PHYSICS_BEGIN

CollisionShape::CollisionShape(QObject* parent)
    : QObject(parent) {
}

CollisionShape::~CollisionShape() {
}

JPH::ShapeRefC CollisionShape::createShape() const {
    if (!isValid()) {
        throw std::invalid_argument("Invalid collision shape configuration");
    }

    const auto result = settings().Create();
    if (!result.IsValid()) {
        throw std::runtime_error(result.HasError()? result.GetError().c_str(): "Jolt did not create a collision shape");
    }
    return result.Get();
}

VOLCANO_PHYSICS_END
