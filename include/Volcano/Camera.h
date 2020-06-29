//
//
#ifndef VOLCANO_CAMERA_H
#define VOLCANO_CAMERA_H

#include <QRect>
#include <QRectF>
#include <QVector3D>
#include <QMatrix4x4>

#include <Volcano/Common.h>
#include <Volcano/Node.h>

VOLCANO_BEGIN

class Camera: public Node
{
    Q_OBJECT
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged)
    Q_PROPERTY(float zNear READ zNear WRITE setZNear NOTIFY zNearChanged)
    Q_PROPERTY(float zFar READ zFar WRITE setZFar NOTIFY zFarChanged)
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVector3D up READ up WRITE setUp NOTIFY upChanged)

public:
    Camera(QObject *parent = nullptr);
    virtual ~Camera(void);

public:
    float fov(void) const;
    void setFov(float v);
    float zNear(void) const;
    void setZNear(float v);
    float zFar(void) const;
    void setZFar(float v);
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &v);
    const QVector3D &up(void) const;
    void setUp(const QVector3D &v);
    QMatrix4x4 viewMatrix(void);

signals:
    void fovChanged(float v);
    void zNearChanged(float v);
    void zFarChanged(float v);
    void directionChanged(const QVector3D &v);
    void upChanged(const QVector3D &v);

private:
    float m_fov;
    float m_zNear;
    float m_zFar;
    QVector3D m_direction;
    QVector3D m_up;
};

VOLCANO_INLINE float Camera::fov(void) const
{
    return m_fov;
}

VOLCANO_INLINE void Camera::setFov(float v)
{
    if (qAbs(m_fov - v) < 0.01)
    {
        m_fov = v;
        fovChanged(v);
    }
}

VOLCANO_INLINE float Camera::zNear(void) const
{
    return m_zNear;
}

VOLCANO_INLINE void Camera::setZNear(float v)
{
    if (qAbs(m_zNear - v) < 0.01)
    {
        m_zNear = v;
        zNearChanged(v);
    }
}

VOLCANO_INLINE float Camera::zFar(void) const
{
    return m_zFar;
}

VOLCANO_INLINE void Camera::setZFar(float v)
{
    if (qAbs(m_zFar - v) < 0.01)
    {
        m_zFar = v;
        zFarChanged(v);
    }
}

VOLCANO_INLINE const QVector3D &Camera::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void Camera::setDirection(const QVector3D &v)
{
    if (m_direction != v)
    {
        m_direction = v;
        setRotation(QQuaternion::fromDirection(m_direction, m_up));
        directionChanged(v);
    }
}

VOLCANO_INLINE const QVector3D &Camera::up(void) const
{
    return m_up;
}

VOLCANO_INLINE void Camera::setUp(const QVector3D &v)
{
    if (m_up != v)
    {
        m_up = v;
        setRotation(QQuaternion::fromDirection(m_direction, m_up));
        upChanged(v);
    }
}

VOLCANO_INLINE QMatrix4x4 Camera::viewMatrix(void)
{
    return transform().inverted();
}

VOLCANO_END

#endif // VOLCANO_CAMERA_H
