#include <stdexcept>

#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

RigidBody::Box::Box(const Eigen::Vector3f& halfExtents) {
    setHalfExtents(halfExtents);
}

void RigidBody::Box::setHalfExtents(const Eigen::Vector3f& value) {
    if (!value.allFinite() || (value.array() <= 0.0f).any()) {
        throw std::invalid_argument("RigidBody box half extents must be finite and positive");
    }
    half_extents_ = value;
}

VOLCANO_WORLD_END
