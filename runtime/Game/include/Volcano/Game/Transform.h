//
//
#ifndef VOLCANO_GAME_TRANSFORM_H
#define VOLCANO_GAME_TRANSFORM_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Component.h>

VOLCANO_GAME_BEGIN

class Transform: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector3D translation READ translation WRITE setTranslation NOTIFY translationChanged FINAL)
    Q_PROPERTY(QVector3D scaling READ scaling WRITE setScaling NOTIFY scalingChanged FINAL)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged FINAL)

public:
    Q_INVOKABLE Transform(QObject* parent = nullptr)
        : QObject(parent) {
    }

    Q_INVOKABLE Transform(const QMatrix4x4& v, QObject* parent = nullptr)
        : QObject(parent)
        , affine_transform_(v) {
    }

public:
    static const Transform& identity();

    Q_INVOKABLE void setToIdentity() {
        affine_transform_.setToIdentity();
    }

    QMatrix4x4& matrix() {
        return affine_transform_;
    }

    const QMatrix4x4& matrix() const {
        return affine_transform_;
    }

    QVector3D translation() const {
        return affine_transform_.translation();
    }

    void setTranslation(const QVector3D& v) {
        if (affine_transform_.translation() != v) {
            affine_transform_.setTranslation(v);
            emit translationChanged(v);
        }
    }

    QVector3D scaling() const {
        return affine_transform_.scaling();
    }

    void setScaling(const QVector3D& v) {
        affine_transform_.setScaling(v);
        emit scalingChanged(v);
    }

    QQuaternion rotation() const {
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
    QVector3D translation_;
    QVector3D scale_;
    QQuaternion rotation_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_TRANSFORM_H
