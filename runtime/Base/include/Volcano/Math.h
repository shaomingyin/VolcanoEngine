//
//
#ifndef EIGEN_H
#define EIGEN_H

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class Transform final {
public:
    Transform() noexcept
        : translation_(0.0f, 0.0f, 0.0f)
        , scale_(1.0f, 1.0f, 1.0f)
        , rotation_(1.0f, 0.0f, 0.0f, 0.0f){
    }

    Transform(const Eigen::Vector3f& translation, const Eigen::Vector3f& scale, const Eigen::Quaternionf& rotation) noexcept
        : translation_(translation)
        , scale_(scale)
        , rotation_(rotation) {
	}

    Transform(const Transform&) = default;

public:
    Transform& operator=(const Transform&) = default;

    Eigen::Vector3f& translation() noexcept {
        return translation_;
    }

    const Eigen::Vector3f& translation() const noexcept {
        return translation_;
    }

    Eigen::Vector3f& scale() noexcept {
        return scale_;
    }

    const Eigen::Vector3f& scale() const noexcept {
        return scale_;
    }

    Eigen::Quaternionf& rotation() noexcept {
        return rotation_;
    }

    const Eigen::Quaternionf& rotation() const noexcept {
        return rotation_;
    }

    Eigen::Affine3f toAffine() const noexcept {
        auto m(Eigen::Affine3f::Identity());
        m.scale(scale_);
        m.rotate(rotation_);
        m.translate(translation_);
        return m;
    }

    void reset() noexcept {
        translation_ = { 0.0f, 0.0f, 0.0f };
        scale_ = { 1.0f, 1.0f, 1.0f };
        rotation_ = { 1.0f, 0.0f, 0.0f, 0.0f };
    }

    Transform& operator*=(const Transform& other) noexcept {
        translation_ += other.translation_;
        //scale *= other.scale;
        rotation_ *= other.rotation_;
        return (*this);
    }

    Transform& operator/=(const Transform& other) noexcept {
        // TODO
        return (*this);
    }

    bool isIdentity() const noexcept {
        return (translation_.isApprox(Eigen::Vector3f(0.0f, 0.0f, 0.0f)) &&
            scale_.isApprox(Eigen::Vector3f(1.0f, 1.0f, 1.0f)) &&
            rotation_.isApprox(Eigen::Quaternionf(1.0f, 0.0f, 0.0f, 0.0f)));
    }

private:
    Eigen::Vector3f translation_ = { 0.0f, 0.0f, 0.0f };
    Eigen::Vector3f scale_ = { 1.0f, 1.0f, 1.0f };
    Eigen::Quaternionf rotation_ = { 0.0f, 0.0f, 0.0f, 1.0f };
};

inline Transform operator*(const Transform& a, const Transform& b) noexcept {
    Transform ret = a;
    ret *= b;
    return ret;
}

inline Transform operator/(const Transform& a, const Transform& b) noexcept {
    Transform ret = a;
    ret /= b;
    return ret;
}

VOLCANO_END

#endif // EIGEN_H
