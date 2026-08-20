//
//
#ifndef VOLCANO_WORLD_CAMERA_H
#define VOLCANO_WORLD_CAMERA_H

#include <QRect>
#include <QMatrix4x4>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class Camera: public Transformable {
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged FINAL)
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged FINAL)
    Q_PROPERTY(float aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged FINAL)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged FINAL)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged FINAL)
    Q_PROPERTY(bool perspective READ isPerspective NOTIFY perspectiveChanged FINAL)

public:
    Camera(QObject* parent = nullptr);

public:
    void reset() noexcept;
    const QMatrix4x4& viewMatrix() const noexcept;
    const QMatrix4x4& projectionMatrix() const noexcept;
    const QRectF& rect() const noexcept;
    void setRect(const QRectF& v) noexcept;
    float fov() const noexcept;
    void setFov(float v) noexcept;
    float aspectRatio() const noexcept;
    void setAspectRatio(float v) noexcept;
    float nearPlane() const noexcept;
    void setNearPlane(float v) noexcept;
    float farPlane() const noexcept;
    void setFarPlane(float v) noexcept;
    bool isPerspective() const noexcept;
    Q_INVOKABLE void lookTo(const QVector3D& pos, const QVector3D& direction, const QVector3D& up) noexcept;
    Q_INVOKABLE void lookAt(const QVector3D& pos, const QVector3D& center, const QVector3D& up) noexcept;
    Q_INVOKABLE void ortho(const QRectF& rect, float near_plane, float far_plane) noexcept;
    Q_INVOKABLE void ortho(float left, float right, float top, float bottom, float near_plane, float far_plane) noexcept;
    void perspective(float fov, float aspect_ratio, float near_plane, float far_plane) noexcept;

signals:
    void rectChanged(const QRectF& v);
    void fovChanged(float v);
    void aspectRatioChanged(float v);
    void nearPlaneChanged(float v);
    void farPlaneChanged(float v);
    void perspectiveChanged(bool v);

private:
    void updateProjection() noexcept;

private:
    QMatrix4x4 view_matrix_;
    QMatrix4x4 projection_matrix_;
    QRectF rect_;
    float fov_;
    float aspect_ratio_;
    float near_plane_;
    float far_plane_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAMERA_H
