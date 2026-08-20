//
//
#include <Volcano/World/Transform.h>

VOLCANO_WORLD_BEGIN

const Transform Transform::Identity;

Transform::Transform(QObject* parent)
    : QObject(parent)
    , translation_(0.0f, 0.0f, 0.0f)
    , scale_(1.0f, 1.0f, 1.0f)
    , rotation_(1.0f, 0.0f, 0.0f, 0.0f) {
}

void Transform::reset() noexcept {
    translation_ = QVector3D(0.0f, 0.0f, 0.0f);
    scale_ = QVector3D(1.0f, 1.0f, 1.0f);
    rotation_ = QQuaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

QMatrix4x4 Transform::toMatrix() const noexcept {
    QMatrix4x4 m;
    m.setToIdentity();
    m.translate(translation_);
    m.rotate(rotation_);
    m.scale(scale_);
    return m;
}

const QVector3D& Transform::translation() const noexcept {
    return translation_;
}

void Transform::setTranslation(const QVector3D& v) noexcept {
    if (translation_ != v) {
        translation_ = v;
        emit translationChanged(v);
    }
}

const QVector3D& Transform::scale() const noexcept {
    return scale_;
}

void Transform::setScale(const QVector3D& v) noexcept {
    if (scale_ != v) {
        scale_ = v;
        emit scaleChanged(v);
    }
}

const QQuaternion& Transform::rotation() const noexcept {
    return rotation_;
}

void Transform::setRotation(const QQuaternion& v) noexcept {
    if (rotation_ != v) {
        rotation_ = v;
        emit rotationChanged(v);
    }
}

bool Transform::fuzzyCompare(const Transform& v) noexcept {
    return qFuzzyCompare(translation_, v.translation_) && qFuzzyCompare(scale_, v.scale_) && qFuzzyCompare(rotation_, v.rotation_);
}

void Transform::copy(const Transform& v) noexcept {
    translation_ = v.translation_;
    scale_ = v.scale_;
    rotation_ = v.rotation_;
}

VOLCANO_WORLD_END
