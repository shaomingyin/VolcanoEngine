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
    Q_PROPERTY(QVector3D translation READ translation WRITE setTranslation NOTIFY translationChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    static const Transform Identity;

public:
    Transform(QObject* parent = nullptr);

public:
    Q_INVOKABLE void reset() noexcept;
    Q_INVOKABLE QMatrix4x4 toMatrix() const noexcept;
    const QVector3D& translation() const noexcept;
    void setTranslation(const QVector3D& v) noexcept;
    const QVector3D& scale() const noexcept;
    void setScale(const QVector3D& v) noexcept;
    const QQuaternion& rotation() const noexcept;
    void setRotation(const QQuaternion& v) noexcept;
    bool fuzzyCompare(const Transform& v) noexcept;
    void copy(const Transform& v) noexcept;

signals:
    void translationChanged(const QVector3D& v);
    void scaleChanged(const QVector3D& v);
    void rotationChanged(const QQuaternion& v);

private:
    QVector3D translation_;
    QVector3D scale_;
    QQuaternion rotation_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORM_H
