#ifndef VOLCANO_TRANSFORM_H
#define VOLCANO_TRANSFORM_H

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

    void setTranslation(const Eigen::Vector3f& value) noexcept {
        translation_ = value;
    }

    constexpr const Eigen::Vector3f &translation() const noexcept {
        return translation_;
    }

    void setScale(const Eigen::Vector3f& value) noexcept {
        scale_ = value;
    }

    constexpr const Eigen::Vector3f &scale() const noexcept {
        return scale_;
    }

    void setRotation(const Eigen::Quaternionf& value) noexcept {
        rotation_ = value;
    }

    constexpr const Eigen::Quaternionf &rotation() const noexcept {
        return rotation_;
    }

    Eigen::Affine3f affine() const;

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
