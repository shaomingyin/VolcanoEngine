//
//
#ifndef VOLCANO_TRANSFORM_HPP
#define VOLCANO_TRANSFORM_HPP

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Transform final {
public:
    Transform()
        : translation_(0.0f, 0.0f, 0.0f)
        , scaling_(1.0f, 1.0f, 1.0f)
        , rotation_(1.0f, 0.0f, 0.0f, 0.0f) {
    }

    Transform(const QVector3D& translation, const QVector3D& scaling, const QQuaternion& rotation)
        : translation_(translation)
        , scaling_(scaling)
        , rotation_(rotation) {
    }

    Transform(const Transform& other)
        : translation_(other.translation_)
        , scaling_(other.scaling_)
        , rotation_(other.rotation_) {
    }

public:
    void reset() {
        operator=(Transform());
    }

    const QVector3D& translation() const {
        return translation_;
    }

    void setTranslation(const QVector3D& v) {
        translation_ = v;
    }

    void translate(const QVector3D& v) {
        translation_ += v;
    }

    const QVector3D& scaling() const {
        return scaling_;
    }

    void setScaling(const QVector3D& v) {
        scaling_ = v;
    }

    void scale(const QVector3D& v) {
        scaling_ *= v;
    }

    const QQuaternion& rotation() const {
        return rotation_;
    }

    void setRotation(const QQuaternion& v) {
        rotation_ = v;
    }

    void rotate(const QQuaternion& v) {
        rotation_ *= v;
    }

    void buildMatrix4x4(QMatrix4x4& v) const {
        v.setToIdentity();
        v.scale(scaling_);
        v.rotate(rotation_);
        v.translate(translation_);
    }

    Transform& operator=(const Transform& other) {
        translation_ = other.translation_;
        scaling_ = other.scaling_;
        rotation_ = other.rotation_;
        return (*this);
    }

    bool operator==(const Transform& other) const {
        return qFuzzyCompare(translation_, other.translation_) &&
               qFuzzyCompare(scaling_, other.scaling_) &&
               qFuzzyCompare(rotation_, rotation_);
    }

    bool operator!=(const Transform& other) const {
        return !operator==(other);
    }

private:
    QVector3D translation_;
    QVector3D scaling_;
    QQuaternion rotation_;
};

VOLCANO_END

#endif // VOLCANO_TRANSFORM_HPP
