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
    Transform()
        : translation_(0.0f, 0.0f, 0.0f)
        , scale_(1.0f, 1.0f, 1.0f)
        , rotation_(1.0f, 0.0f, 0.0f, 0.0f) {
    }

    Transform(const Transform&) = default;

public:
    Transform& operator=(const Transform&) = default;

    Eigen::Affine3f toMatrix() const noexcept {
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
