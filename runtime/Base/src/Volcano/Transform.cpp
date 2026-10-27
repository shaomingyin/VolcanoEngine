#include <Volcano/Math.h>
#include <Volcano/Transform.h>

VOLCANO_BEGIN

Eigen::Matrix3f Transform::normalMatrix() const {
    return Eigen::normalMatrix(affine());
}

VOLCANO_END
