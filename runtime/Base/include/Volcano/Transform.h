#ifndef VOLCANO_TRANSFORM_H
#define VOLCANO_TRANSFORM_H

#include <stdexcept>

#include <Eigen/Geometry>

#include <Volcano/Common.h>

VOLCANO_BEGIN

// Stores editable translation, scale and rotation without a cached matrix.
class Transform final {
public:
    Transform() = default;

    Transform(const Eigen::Vector3f &translation, const Eigen::Vector3f &scale, const Eigen::Quaternionf &rotation) noexcept
        : translation_(translation), scale_(scale), rotation_(rotation) {
    }

    Eigen::Vector3f &translation() noexcept {
        return translation_;
    }

    const Eigen::Vector3f &translation() const noexcept {
        return translation_;
    }

    Eigen::Vector3f &scale() noexcept {
        return scale_;
    }

    const Eigen::Vector3f &scale() const noexcept {
        return scale_;
    }

    Eigen::Quaternionf &rotation() noexcept {
        return rotation_;
    }

    const Eigen::Quaternionf &rotation() const noexcept {
        return rotation_;
    }

    Eigen::Affine3f affine() const {
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

    Eigen::Matrix3f normalMatrix() const;

    void reset() noexcept {
        *this = Transform();
    }

private:
    Eigen::Vector3f translation_ = Eigen::Vector3f::Zero();
    Eigen::Vector3f scale_ = Eigen::Vector3f::Ones();
    Eigen::Quaternionf rotation_ = Eigen::Quaternionf::Identity();
};

VOLCANO_END

#endif // VOLCANO_TRANSFORM_H
