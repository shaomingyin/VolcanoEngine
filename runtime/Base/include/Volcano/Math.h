//
//
#ifndef VOLCANO_MATH_H
#define VOLCANO_MATH_H

#include <cmath>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <QMetaType>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class Vector2 final {
    Q_GADGET
    Q_PROPERTY(float x READ x WRITE setX FINAL)
    Q_PROPERTY(float y READ y WRITE setY FINAL)

public:
    Vector2() = default;
    Vector2(const Vector2&) = default;

    Vector2(float x, float y)
        : v_(x, y) {
    }

    Vector2(const Eigen::Vector2f& v)
        : v_(v) {
    }

    Vector2& operator=(const Vector2&) = default;

    Vector2& operator=(const Eigen::Vector2f& v) {
        v_ = v;
        return (*this);
    }

    bool operator==(const Vector2& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Vector2f& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Vector2& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Vector2f& v) const {
        return !v_.isApprox(v);
    }

    Vector2& operator+=(const Vector2& v) {
        v_ += v.v_;
        return (*this);
    }

    Vector2& operator+=(const Eigen::Vector2f& v) {
        v_ += v;
        return (*this);
    }

    friend Vector2 operator+(const Vector2& a, const Vector2& b) {
        return Vector2(a.v_ + b.v_);
    }

    friend Vector2 operator+(const Vector2& a, const Eigen::Vector2f& b) {
        return Vector2(a.v_ + b);
    }

    Vector2& operator-=(const Vector2& v) {
        v_ -= v.v_;
        return (*this);
    }

    Vector2& operator-=(const Eigen::Vector2f& v) {
        v_ -= v;
        return (*this);
    }

    friend Vector2 operator-(const Vector2& a, const Vector2& b) {
        return Vector2(a.v_ - b.v_);
    }

    friend Vector2 operator-(const Vector2& a, const Eigen::Vector2f& b) {
        return Vector2(a.v_ - b);
    }

    Vector2& operator*=(float v) {
        v_ *= v;
        return (*this);
    }

    Vector2& operator*=(const Vector2& v) {
        v_.cwiseProduct(v.v_);
        return (*this);
    }

    Vector2& operator*=(const Eigen::Vector2f& v) {
        v_.cwiseProduct(v);
        return (*this);
    }

    friend Vector2 operator*(const Vector2& a, float v) {
        return Vector2(a.v_ * v);
    }

    Vector2& operator/=(float v) {
        v_ /= v;
        return (*this);
    }

    friend Vector2 operator/(const Vector2& a, float v) {
        return Vector2(a.v_ / v);
    }

    const float* data() const {
        return v_.data();
    }

    const Eigen::Vector2f& toEigen() const {
        return v_;
    }

    float x() const {
        return v_.x();
    }

    void setX(float v) {
        v_.x() = v;
    }

    float y() const {
        return v_.x();
    }

    void setY(float v) {
        v_.y() = v;
    }

    Q_INVOKABLE void normalize() {
        v_.normalize();
    }

private:
    Eigen::Vector2f v_;
};

class Vector3 final {
    Q_GADGET
    Q_PROPERTY(float x READ x WRITE setX FINAL)
    Q_PROPERTY(float y READ y WRITE setY FINAL)
    Q_PROPERTY(float z READ z WRITE setZ FINAL)

public:
    Vector3() = default;
    Vector3(const Vector3&) = default;
    Vector3& operator=(const Vector3&) = default;

    Vector3(float x, float y, float z)
        : v_(x, y, z) {
    }

    Vector3(const Eigen::Vector3f& v)
        : v_(v) {
    }

    Vector3& operator=(const Eigen::Vector3f& v) {
        v_ = v;
        return (*this);
    }

    bool operator==(const Vector3& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Vector3f& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Vector3& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Vector3f& v) const {
        return !v_.isApprox(v);
    }

    Vector3& operator+=(const Vector3& v) {
        v_ += v.v_;
        return (*this);
    }

    Vector3& operator+=(const Eigen::Vector3f& v) {
        v_ += v;
        return (*this);
    }

    friend Vector3 operator+(const Vector3& a, const Vector3& b) {
        return Vector3(a.v_ + b.v_);
    }

    friend Vector3 operator+(const Vector3& a, const Eigen::Vector3f& b) {
        return Vector3(a.v_ + b);
    }

    Vector3& operator-=(const Vector3& v) {
        v_ -= v.v_;
        return (*this);
    }

    Vector3& operator-=(const Eigen::Vector3f& v) {
        v_ -= v;
        return (*this);
    }

    friend Vector3 operator-(const Vector3& a, const Vector3& b) {
        return Vector3(a.v_ - b.v_);
    }

    friend Vector3 operator-(const Vector3& a, const Eigen::Vector3f& b) {
        return Vector3(a.v_ - b);
    }

    Vector3& operator*=(float v) {
        v_ *= v;
        return (*this);
    }

    Vector3& operator*=(const Vector3& v) {
        v_.cross(v.v_);
        return (*this);
    }

    Vector3& operator*=(const Eigen::Vector3f& v) {
        v_.cross(v);
        return (*this);
    }

    friend Vector3 operator*(const Vector3& a, float v) {
        return Vector3(a.v_ * v);
    }

    friend Vector3 operator*(const Vector3& a, const Vector3& b) {
        return Vector3(a.v_.cross(b.v_));
    }

    friend Vector3 operator*(const Vector3& a, const Eigen::Vector3f& b) {
        return Vector3(a.v_.cross(b));
    }

    Vector3& operator/=(float v) {
        v_ /= v;
        return (*this);
    }

    friend Vector3 operator/(const Vector3& a, float v) {
        return Vector3(a.v_ / v);
    }

    const float* data() const {
        return v_.data();
    }

    const Eigen::Vector3f& toEigen() const {
        return v_;
    }

    float x() const {
        return v_.x();
    }

    void setX(float v) {
        v_.x() = v;
    }

    float y() const {
        return v_.x();
    }

    void setY(float v) {
        v_.y() = v;
    }

    float z() const {
        return v_.z();
    }

    void setZ(float v) {
        v_.z() = v;
    }

    Q_INVOKABLE void normalize() {
        v_.normalize();
    }

    Q_INVOKABLE Vector2 toVector2() const {
        return Vector2(x(), y());
    }

private:
    Eigen::Vector3f v_;
};

class Vector4 final {
    Q_GADGET
    Q_PROPERTY(float x READ x WRITE setX FINAL)
    Q_PROPERTY(float y READ y WRITE setY FINAL)
    Q_PROPERTY(float z READ z WRITE setZ FINAL)
    Q_PROPERTY(float w READ w WRITE setW FINAL)

public:
    Vector4() = default;
    Vector4(const Vector4&) = default;
    Vector4& operator=(const Vector4&) = default;

    Vector4(float x, float y, float z, float w)
        : v_(x, y, z, w) {
    }

    Vector4(const Eigen::Vector4f& v)
        : v_(v) {
    }

    Vector4& operator=(const Eigen::Vector4f& v) {
        v_ = v;
        return (*this);
    }

    bool operator==(const Vector4& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Vector4f& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Vector4& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Vector4f& v) const {
        return !v_.isApprox(v);
    }

    Vector4& operator+=(const Vector4& v) {
        v_ += v.v_;
        return (*this);
    }

    Vector4& operator+=(const Eigen::Vector4f& v) {
        v_ += v;
        return (*this);
    }

    friend Vector4 operator+(const Vector4& a, const Vector4& b) {
        return Vector4(a.v_ + b.v_);
    }

    friend Vector4 operator+(const Vector4& a, const Eigen::Vector4f& b) {
        return Vector4(a.v_ + b);
    }

    Vector4& operator-=(const Vector4& v) {
        v_ -= v.v_;
        return (*this);
    }

    Vector4& operator-=(const Eigen::Vector4f& v) {
        v_ -= v;
        return (*this);
    }

    friend Vector4 operator-(const Vector4& a, const Vector4& b) {
        return Vector4(a.v_ - b.v_);
    }

    friend Vector4 operator-(const Vector4& a, const Eigen::Vector4f& b) {
        return Vector4(a.v_ - b);
    }

    Vector4& operator*=(float v) {
        v_ *= v;
        return (*this);
    }

    friend Vector4 operator*(const Vector4& a, float v) {
        return Vector4(a.v_ * v);
    }

    Vector4& operator/=(float v) {
        v_ /= v;
        return (*this);
    }

    friend Vector4 operator/(const Vector4& a, float v) {
        return Vector4(a.v_ / v);
    }

    const float* data() const {
        return v_.data();
    }

    const Eigen::Vector4f& toEigen() const {
        return v_;
    }

    float x() const {
        return v_.x();
    }

    void setX(float v) {
        v_.x() = v;
    }

    float y() const {
        return v_.x();
    }

    void setY(float v) {
        v_.y() = v;
    }

    float z() const {
        return v_.z();
    }

    void setZ(float v) {
        v_.z() = v;
    }

    float w() const {
        return v_.w();
    }

    void setW(float v) {
        v_.w() = v;
    }

    Q_INVOKABLE void normalize() {
        v_.normalize();
    }

    Q_INVOKABLE Vector2 toVector2() const {
        return Vector2(v_.x(), v_.y());
    }

    Q_INVOKABLE Vector3 toVector3() const {
        return Vector3(v_.x(), v_.y(), v_.z());
    }

private:
    Eigen::Vector4f v_;
};

class Matrix2 final {
    Q_GADGET

public:
    Matrix2() = default;
    Matrix2(const Matrix2&) = default;
    Matrix2& operator=(const Matrix2&) = default;

    Matrix2(const Eigen::Matrix2f& v)
        : v_(v) {
    }

    Matrix2& operator=(const Eigen::Matrix2f& v) {
        v_ = v;
        return (*this);
    }

    bool operator==(const Matrix2& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Matrix2f& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Matrix2& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Matrix2f& v) const {
        return !v_.isApprox(v);
    }

    Matrix2& operator+=(const Matrix2& v) {
        v_ += v.v_;
        return (*this);
    }

    Matrix2& operator+=(const Eigen::Matrix2f& v) {
        v_ += v;
        return (*this);
    }

    friend Matrix2 operator+(const Matrix2& a, const Matrix2& b) {
        return Matrix2(a.v_ + b.v_);
    }

    friend Matrix2 operator+(const Matrix2& a, const Eigen::Matrix2f& b) {
        return Matrix2(a.v_ + b);
    }

    Matrix2& operator-=(const Matrix2& v) {
        v_ -= v.v_;
        return (*this);
    }

    Matrix2& operator-=(const Eigen::Matrix2f& v) {
        v_ -= v;
        return (*this);
    }

    friend Matrix2 operator-(const Matrix2& a, const Matrix2& b) {
        return Matrix2(a.v_ - b.v_);
    }

    friend Matrix2 operator-(const Matrix2& a, const Eigen::Matrix2f& b) {
        return Matrix2(a.v_ - b);
    }

    Matrix2& operator*=(float v) {
        v_ *= v;
        return (*this);
    }

    Matrix2& operator*=(const Matrix2& v) {
        v_ *= v.v_;
        return (*this);
    }

    Matrix2& operator*=(const Eigen::Matrix2f& v) {
        v_ *= v;
        return (*this);
    }

    friend Matrix2 operator*(const Matrix2& a, float v) {
        return Matrix2(a.v_ * v);
    }

    friend Matrix2 operator*(const Matrix2& a, const Matrix2& b) {
        return Matrix2(a.v_ * b.v_);
    }

    friend Matrix2 operator*(const Matrix2& a, const Eigen::Matrix2f& b) {
        return Matrix2(a.v_ * b);
    }

    friend Vector2 operator*(const Matrix2& a, const Vector2& b) {
        return Vector2(a.v_ * b.toEigen());
    }

    friend Vector2 operator*(const Matrix2& a, const Eigen::Vector2f& b) {
        return Vector2(a.v_ * b);
    }

    Matrix2& operator/=(float v) {
        v_ /= v;
        return (*this);
    }

    friend Matrix2 operator/(const Matrix2& a, float v) {
        return Matrix2(a.v_ / v);
    }

    const float* data() const {
        return v_.data();
    }

    const Eigen::Matrix2f& toEigen() const {
        return v_;
    }

private:
    Eigen::Matrix2f v_;
};

class Matrix3 final {
    Q_GADGET

public:
    Matrix3() = default;
    Matrix3(const Matrix3&) = default;
    Matrix3& operator=(const Matrix3&) = default;

    Matrix3(const Eigen::Matrix3f& v)
        : v_(v) {
    }

    Matrix3& operator=(const Eigen::Matrix3f& v) {
        v_ = v;
        return (*this);
    }

    bool operator==(const Matrix3& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Matrix3f& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Matrix3& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Matrix3f& v) const {
        return !v_.isApprox(v);
    }

    Matrix3& operator+=(const Matrix3& v) {
        v_ += v.v_;
        return (*this);
    }

    Matrix3& operator+=(const Eigen::Matrix3f& v) {
        v_ += v;
        return (*this);
    }

    friend Matrix3 operator+(const Matrix3& a, const Matrix3& b) {
        return Matrix3(a.v_ + b.v_);
    }

    friend Matrix3 operator+(const Matrix3& a, const Eigen::Matrix3f& b) {
        return Matrix3(a.v_ + b);
    }

    Matrix3& operator-=(const Matrix3& v) {
        v_ -= v.v_;
        return (*this);
    }

    Matrix3& operator-=(const Eigen::Matrix3f& v) {
        v_ -= v;
        return (*this);
    }

    friend Matrix3 operator-(const Matrix3& a, const Matrix3& b) {
        return Matrix3(a.v_ - b.v_);
    }

    friend Matrix3 operator-(const Matrix3& a, const Eigen::Matrix3f& b) {
        return Matrix3(a.v_ - b);
    }

    Matrix3& operator*=(float v) {
        v_ *= v;
        return (*this);
    }

    Matrix3& operator*=(const Matrix3& v) {
        v_ *= v.v_;
        return (*this);
    }

    Matrix3& operator*=(const Eigen::Matrix3f& v) {
        v_ *= v;
        return (*this);
    }

    friend Matrix3 operator*(const Matrix3& a, float v) {
        return Matrix3(a.v_ * v);
    }

    friend Matrix3 operator*(const Matrix3& a, const Matrix3& b) {
        return Matrix3(a.v_ * b.v_);
    }

    friend Matrix3 operator*(const Matrix3& a, const Eigen::Matrix3f& b) {
        return Matrix3(a.v_ * b);
    }

    friend Vector3 operator*(const Matrix3& a, const Vector3& b) {
        return Vector3(a.v_ * b.toEigen());
    }

    friend Vector3 operator*(const Matrix3& a, const Eigen::Vector3f& b) {
        return Vector3(a.v_ * b);
    }

    Matrix3& operator/=(float v) {
        v_ /= v;
        return (*this);
    }

    friend Matrix3 operator/(const Matrix3& a, float v) {
        return Matrix3(a.v_ / v);
    }

    const float* data() const {
        return v_.data();
    }

    const Eigen::Matrix3f& toEigen() const {
        return v_;
    }

private:
    Eigen::Matrix3f v_;
};

class Matrix4 final {
    Q_GADGET

public:
    Matrix4() = default;
    Matrix4(const Matrix4&) = default;
    Matrix4& operator=(const Matrix4&) = default;

    Matrix4(const Eigen::Matrix4f& v)
        : v_(v) {
    }

    Matrix4& operator=(const Eigen::Matrix4f& v) {
        v_ = v;
        return (*this);
    }

    bool operator==(const Matrix4& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Matrix4f& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Matrix4& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Matrix4f& v) const {
        return !v_.isApprox(v);
    }

    Matrix4& operator+=(const Matrix4& v) {
        v_ += v.v_;
        return (*this);
    }

    Matrix4& operator+=(const Eigen::Matrix4f& v) {
        v_ += v;
        return (*this);
    }

    friend Matrix4 operator+(const Matrix4& a, const Matrix4& b) {
        return Matrix4(a.v_ + b.v_);
    }

    friend Matrix4 operator+(const Matrix4& a, const Eigen::Matrix4f& b) {
        return Matrix4(a.v_ + b);
    }

    Matrix4& operator-=(const Matrix4& v) {
        v_ -= v.v_;
        return (*this);
    }

    Matrix4& operator-=(const Eigen::Matrix4f& v) {
        v_ -= v;
        return (*this);
    }

    friend Matrix4 operator-(const Matrix4& a, const Matrix4& b) {
        return Matrix4(a.v_ - b.v_);
    }

    friend Matrix4 operator-(const Matrix4& a, const Eigen::Matrix4f& b) {
        return Matrix4(a.v_ - b);
    }

    Matrix4& operator*=(float v) {
        v_ *= v;
        return (*this);
    }

    Matrix4& operator*=(const Matrix4& v) {
        v_ *= v.v_;
        return (*this);
    }

    Matrix4& operator*=(const Eigen::Matrix4f& v) {
        v_ *= v;
        return (*this);
    }

    friend Matrix4 operator*(const Matrix4& a, float v) {
        return Matrix4(a.v_ * v);
    }

    friend Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
        return Matrix4(a.v_ * b.v_);
    }

    friend Matrix4 operator*(const Matrix4& a, const Eigen::Matrix4f& b) {
        return Matrix4(a.v_ * b);
    }

    friend Vector4 operator*(const Matrix4& a, const Vector4& b) {
        return Vector4(a.v_ * b.toEigen());
    }

    friend Vector4 operator*(const Matrix4& a, const Eigen::Vector4f& b) {
        return Vector4(a.v_ * b);
    }

    Matrix4& operator/=(float v) {
        v_ /= v;
        return (*this);
    }

    friend Matrix4 operator/(const Matrix4& a, float v) {
        return Matrix4(a.v_ / v);
    }

    const float* data() const {
        return v_.data();
    }

    const Eigen::Matrix4f& toEigen() const {
        return v_;
    }

private:
    Eigen::Matrix4f v_;
};

class AngleAxis final {
    Q_GADGET
    Q_PROPERTY(float angle READ angle WRITE setAngle FINAL)
    Q_PROPERTY(Vector3 axis READ axis WRITE setAxis FINAL)

public:
    AngleAxis() = default;
    AngleAxis(const AngleAxis&) = default;
    AngleAxis& operator=(const AngleAxis&) = default;

    AngleAxis(const Eigen::Matrix3f& v)
        : v_(v) {
    }

    AngleAxis(const Matrix3& v)
        : v_(v.toEigen()) {
    }

    AngleAxis(const Eigen::AngleAxisf& v)
        : v_(v) {
    }

    AngleAxis& operator=(const Eigen::AngleAxisf& v) {
        v_ = v;
        return (*this);
    }

    bool operator==(const AngleAxis& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::AngleAxisf& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const AngleAxis& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::AngleAxisf& v) const {
        return !v_.isApprox(v);
    }

    const Eigen::AngleAxisf& toEigen() const {
        return v_;
    }

    float angle() const {
        return v_.angle();
    }

    void setAngle(float v) {
        v_.angle() = v;
    }

    Vector3 axis() const {
        return Vector3(v_.axis());
    }

    void setAxis(const Vector3& v) {
        v_.axis() = v.toEigen();
    }

private:
    Eigen::AngleAxisf v_;
};

class Quaternion final {
    Q_GADGET
    Q_PROPERTY(float x READ x WRITE setX FINAL)
    Q_PROPERTY(float y READ y WRITE setY FINAL)
    Q_PROPERTY(float z READ z WRITE setZ FINAL)
    Q_PROPERTY(float w READ w WRITE setW FINAL)

public:
    Quaternion() = default;
    Quaternion(const Quaternion&) = default;
    Quaternion& operator=(const Quaternion&) = default;

    Quaternion(float w, float x, float y, float z)
        : v_(w, x, y, z) {
    }

    Quaternion(const Eigen::Quaternionf& v)
        : v_(v) {
    }

    bool operator==(const Quaternion& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Quaternionf& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Quaternion& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Quaternionf& v) const {
        return !v_.isApprox(v);
    }

    float x() const {
        return v_.x();
    }

    void setX(float v) {
        v_.x() = v;
    }

    float y() const {
        return v_.x();
    }

    void setY(float v) {
        v_.y() = v;
    }

    float z() const {
        return v_.z();
    }

    void setZ(float v) {
        v_.z() = v;
    }

    float w() const {
        return v_.w();
    }

    void setW(float v) {
        v_.w() = v;
    }

    const Eigen::Quaternionf& toEigen() const {
        return v_;
    }

    Q_INVOKABLE void normalize() {
        v_.normalize();
    }

    Q_INVOKABLE Matrix3 toRotationMatrix() const {
        return v_.toRotationMatrix();
    }

private:
    Eigen::Quaternionf v_;
};

class Affine2 final {
    Q_GADGET
    Q_PROPERTY(Vector2 translation READ translation WRITE setTranslation FINAL)
    Q_PROPERTY(Vector2 scaling READ scaling WRITE setScaling FINAL)
    Q_PROPERTY(Matrix2 rotation READ rotation WRITE setRotation FINAL)

public:
    Affine2() = default;
    Affine2(const Affine2&) = default;
    Affine2& operator=(const Affine2&) = default;

    Affine2(const Eigen::Affine2f& v)
        : v_(v) {
    }

    bool operator==(const Affine2& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Affine2f& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Affine2& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Affine2f& v) const {
        return !v_.isApprox(v);
    }

    friend Affine2 operator*(const Affine2& a, const Affine2& b) {
        return Affine2(a.v_ * b.v_);
    }

    friend Vector2 operator*(const Affine2& a, const Vector2& b) {
        return Vector2(a.v_ * b.toEigen());
    }

    const float* data() const {
        return v_.data();
    }

    const Eigen::Affine2f& toEigen() const {
        return v_;
    }

    Vector2 translation() const {
        return Vector2(v_.translation());
    }

    void setTranslation(const Vector2& v) {
        v_.translate(v.toEigen());
    }

    Vector2 scaling() const {
        auto linear = v_.linear();
        return Vector2(linear.col(0).norm(), linear.col(1).norm());
    }

    void setScaling(const Vector2& v) {
        v_.scale(v.toEigen());
    }

    Matrix2 rotation() const {
        return Matrix2(v_.rotation());
    }

    void setRotation(const Matrix2& v) {
        v_.rotate(v.toEigen());
    }

    Q_INVOKABLE void setToIdentity() {
        v_ = Eigen::Affine2f::Identity();
    }

private:
    Eigen::Affine2f v_;
};

class Affine3 final {
    Q_GADGET
    Q_PROPERTY(Vector3 translation READ translation WRITE setTranslation FINAL)
    Q_PROPERTY(Vector3 scaling READ scaling WRITE setScaling FINAL)
    Q_PROPERTY(Matrix3 rotation READ rotation WRITE setRotation FINAL)

public:
    Affine3() = default;
    Affine3(const Affine3&) = default;
    Affine3& operator=(const Affine3&) = default;

    Affine3(const Eigen::Affine3f& v)
        : v_(v) {
    }

    bool operator==(const Affine3& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Affine3f& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Affine3& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Affine3f& v) const {
        return !v_.isApprox(v);
    }

    friend Affine3 operator*(const Affine3& a, const Affine3& b) {
        return Affine3(a * b);
    }

    friend Vector3 operator*(const Affine3& a, const Vector3& b) {
        return Vector3(a * b);
    }

    const float* data() const {
        return v_.data();
    }

    const Eigen::Affine3f& toEigen() const {
        return v_;
    }

    Vector3 translation() const {
        return Vector3(v_.translation());
    }

    void setTranslation(const Vector3& v) {
        v_.translate(v.toEigen());
    }

    Vector3 scaling() const {
        auto linear = v_.linear();
        return Vector3(linear.col(0).norm(), linear.col(1).norm(), linear.col(2).norm());
    }

    void setScaling(const Vector3& v) {
        v_.scale(v.toEigen());
    }

    Matrix3 rotation() const {
        return Matrix3(v_.rotation());
    }

    void setRotation(const Matrix3& v) {
        v_.rotate(v.toEigen());
    }

    void setRotation(const AngleAxis& v) {
        v_.rotate(v.toEigen());
    }

    Q_INVOKABLE void setToIdentity() {
        v_ = Eigen::Affine3f::Identity();
    }

    Q_INVOKABLE Affine3 inverted() const {
        return v_.inverse();
    }

private:
    Eigen::Affine3f v_;
};

class Projective3 final {
    Q_GADGET

public:
    Projective3() = default;
    Projective3(const Projective3&) = default;
    Projective3& operator=(const Projective3&) = default;

    Projective3(const Eigen::Projective3f& v)
        : v_(v) {
    }

    bool operator==(const Projective3& v) const {
        return v_.isApprox(v.toEigen());
    }

    bool operator==(const Eigen::Projective3f& v) const {
        return v_.isApprox(v);
    }

    bool operator!=(const Projective3& v) const {
        return !v_.isApprox(v.toEigen());
    }

    bool operator!=(const Eigen::Projective3f& v) const {
        return !v_.isApprox(v);
    }

    const float* data() const {
        return v_.data();
    }

    const Eigen::Projective3f& toEigen() const {
        return v_;
    }

    Q_INVOKABLE void frustum(float left, float right, float bottom, float top, float near_plane, float far_plane) {
    }

    Q_INVOKABLE void ortho(float left, float right, float bottom, float top, float near_plane, float far_plane) {
    }

    Q_INVOKABLE void perspective(float vfov, float aspect_ratio, float near_plane, float far_plane) {
    }

private:
    Eigen::Projective3f v_;
};

VOLCANO_END

Q_DECLARE_METATYPE(Volcano::Vector2)
Q_DECLARE_METATYPE(Volcano::Vector3)
Q_DECLARE_METATYPE(Volcano::Vector4)
Q_DECLARE_METATYPE(Volcano::AngleAxis)
Q_DECLARE_METATYPE(Volcano::Quaternion)
Q_DECLARE_METATYPE(Volcano::Affine3)
Q_DECLARE_METATYPE(Volcano::Projective3)

#endif // VOLCANO_MATH_H
