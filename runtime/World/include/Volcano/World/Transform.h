//
//
#ifndef VOLCANO_WORLD_TRANSFORM_H
#define VOLCANO_WORLD_TRANSFORM_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QObject>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Transform: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector3D translation READ translation WRITE setTranslation NOTIFY translationChanged FINAL)
    Q_PROPERTY(QVector3D scaling READ scaling WRITE setScaling NOTIFY scalingChanged FINAL)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged FINAL)

public:
    static const Transform Identity;

public:
    Transform(QObject* parent = nullptr);

public:
    Q_INVOKABLE void setToIdentity() noexcept;
    Q_INVOKABLE QMatrix4x4 toMatrix() const noexcept;

    const QVector3D& translation() const noexcept {
        return translation_;
    }

    void setTranslation(const QVector3D& v) noexcept {
        if (translation_ != v) {
            translation_ = v;
            emit translationChanged(v);
        }
    }

    const QVector3D& scale() const noexcept {
        return scale_;
    }

    void setScale(const QVector3D& v) noexcept {
        if (scale_ != v) {
            scale_ = v;
            emit scalingChanged(v);
        }
    }

    const QQuaternion& rotation() const noexcept {
        return rotation_;
    }

    void setRotation(const QQuaternion& v) noexcept {
        if (rotation_ != v) {
            rotation_ = v;
            emit rotationChanged(v);
        }
    }

signals:
    void translationChanged(const QVector3D& v);
    void scalingChanged(const QVector3D& v);
    void rotationChanged(const QQuaternion& v);

private:
    QVector3D translation_;
    QVector3D scale_;
    QQuaternion rotation_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORM_H
