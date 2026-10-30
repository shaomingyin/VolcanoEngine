#include <stdexcept>

#include <Volcano/Math.h>
#include <Volcano/Transform.h>

VOLCANO_BEGIN

Eigen::Affine3f Transform::affine() const {
    // S * R * T applies translation, then rotation, then scaling to column vectors.
    Eigen::Affine3f result = Eigen::Affine3f::Identity();
    result.scale(scale_);
    result.rotate(rotation_);
    result.translate(translation_);
    if (!result.matrix().allFinite()) {
        throw std::invalid_argument("Transform must be finite");
    }

    return result;
}

Eigen::Matrix3f Transform::normalMatrix() const {
    return Eigen::normalMatrix(affine());
}

VOLCANO_END
