//
//
#ifndef VOLCANO_WORLD_TRANSFORM_H
#define VOLCANO_WORLD_TRANSFORM_H

#include <QObject>

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>

VOLCANO_WORLD_BEGIN

class Transform: public QObject {
    Q_OBJECT
    Q_PROPERTY(Vector3 translation READ translation WRITE setTranslation NOTIFY translationChanged FINAL)
    Q_PROPERTY(Vector3 scaling READ scaling WRITE setScaling NOTIFY scalingChanged FINAL)
    Q_PROPERTY(AngleAxis rotation READ rotation WRITE setRotation NOTIFY rotationChanged FINAL)

public:
    Q_INVOKABLE Transform(QObject* parent = nullptr)
        : QObject(parent) {
    }

    Q_INVOKABLE Transform(const Affine3& v, QObject* parent = nullptr)
        : QObject(parent)
        , affine_transform_(v) {
    }

public:
    Q_INVOKABLE void setToIdentity() {
        affine_transform_.setToIdentity();
    }

    Affine3& affine() {
        return affine_transform_;
    }

    const Affine3& affine() const {
        return affine_transform_;
    }

    Vector3 translation() const {
        return affine_transform_.translation();
    }

    void setTranslation(const Vector3& v) {
        if (affine_transform_.translation() != v) {
            affine_transform_.setTranslation(v);
            emit translationChanged(v);
        }
    }

    Vector3 scaling() const {
        return affine_transform_.scaling();
    }

    void setScaling(const Vector3& v) {
        affine_transform_.setScaling(v);
        emit scalingChanged(v);
    }

    AngleAxis rotation() const {
        return AngleAxis(affine_transform_.rotation());
    }

    void setRotation(const AngleAxis& v) {
        if (rotation() != v) {
            affine_transform_.setRotation(v);
            emit rotationChanged(v);
        }
    }

    Q_INVOKABLE Transform inverted() const {
        return Transform(affine_transform_.inverted());
    }

public:
    Transform& operator=(const Transform& other) {
        if (this != &other) {
            affine_transform_ = other.affine_transform_;
        }
        return (*this);
    }

    Transform& operator=(const Affine3& v) {
        affine_transform_ = v;
        return (*this);
    }

    friend Transform operator*(const Transform& a, const Transform& b) {
        return Transform(a.affine_transform_ * b.affine_transform_);
    }

    friend Vector3 operator*(const Transform& a, const Vector3& b) {
        return (a.affine_transform_ * b);
    }

signals:
    void translationChanged(const Vector3& v);
    void scalingChanged(const Vector3& v);
    void rotationChanged(const AngleAxis& v);

private:
    Affine3 affine_transform_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORM_H
