//
//
#ifndef VOLCANO_RENDERER_CAMERA_H
#define VOLCANO_RENDERER_CAMERA_H

#include <QRect>
#include <QRectF>
#include <QVector3D>
#include <QMatrix4x4>

#include <Volcano/Object.h>
#include <Volcano/Renderer/Common.h>

VOLCANO_RENDERER_BEGIN

class VOLCANO_API Camera: public Object
{
    Q_OBJECT
    Q_PROPERTY(QRect viewport READ viewport WRITE setViewport NOTIFY viewportChanged)
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged)
    Q_PROPERTY(float zNear READ zNear WRITE setZNear NOTIFY zNearChanged)
    Q_PROPERTY(float zFar READ zFar WRITE setZFar NOTIFY zFarChanged)
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVector3D up READ up WRITE setUp NOTIFY upChanged)

public:
    Camera(QObject *parent = nullptr);
    virtual ~Camera(void);

public:
    const QRect &viewport(void) const;
    void setViewport(const QRect &v);
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
    const QMatrix4x4 &projectMatrix(void) const;

signals:
    void viewportChanged(const QRect &v);
    void fovChanged(float v);
    void zNearChanged(float v);
    void zFarChanged(float v);
    void directionChanged(const QVector3D &v);
    void upChanged(const QVector3D &v);
    void viewMatrixChanged(const QMatrix4x4 &v);
    void projectMatrixChanged(const QMatrix4x4 &v);

private:
    QRect m_viewport;
    float m_fov;
    float m_zNear;
    float m_zFar;
    QVector3D m_direction;
    QVector3D m_up;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectMatrix;
};

VOLCANO_INLINE const QRect &Camera::viewport(void) const
{
    return m_viewport;
}

VOLCANO_INLINE void Camera::setViewport(const QRect &v)
{
    if (m_viewport != v)
    {
        m_viewport = v;
        viewportChanged(v);
    }
}

VOLCANO_INLINE float Camera::fov(void) const
{
    return m_fov;
}

VOLCANO_INLINE void Camera::setFov(float v)
{
    if (qAbs(m_fov - v) < 0.01)
    {
        m_fov = v;
        m_projectMatrix.perspective(v, m_viewport.width() / m_viewport.height(), m_zNear, m_zFar);
        fovChanged(v);
        projectMatrixChanged(m_projectMatrix);
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
        m_projectMatrix.perspective(v, m_viewport.width() / m_viewport.height(), m_zNear, m_zFar);
        zNearChanged(v);
        projectMatrixChanged(m_projectMatrix);
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
        m_projectMatrix.perspective(v, m_viewport.width() / m_viewport.height(), m_zNear, m_zFar);
        zFarChanged(v);
        projectMatrixChanged(m_projectMatrix);
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
        m_viewMatrix.lookAt(position(), v, m_up);
        directionChanged(v);
        viewMatrixChanged(m_viewMatrix);
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
        m_viewMatrix.lookAt(position(), m_direction, v);
        upChanged(v);
        viewMatrixChanged(m_viewMatrix);
    }
}

VOLCANO_INLINE QMatrix4x4 Camera::viewMatrix(void)
{
    return transform().inverted();
}

VOLCANO_INLINE const QMatrix4x4 &Camera::projectMatrix(void) const
{
    return m_projectMatrix;
}

VOLCANO_RENDERER_END

#endif // VOLCANO_RENDERER_CAMERA_H
