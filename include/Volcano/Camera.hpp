//
//
#ifndef VOLCANO_CAMERA_HPP
#define VOLCANO_CAMERA_HPP

#include <QRectF>
#include <QVector3D>
#include <QMatrix4x4>

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

class VOLCANO_API Camera : public Node {
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVector3D up READ up WRITE setUp NOTIFY upChanged)

public:
	Camera(Node *parent = nullptr);
	~Camera(void) override;

public:
    const QRectF &rect(void) const;
    void setRect(const QRectF &v);
    float nearPlane(void) const;
    void setNearPlane(float v);
    float farPlane(void) const;
    void setFarPlane(float v);
    float fov(void) const;
    void setFov(float v);
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &v);
    const QVector3D &up(void) const;
    void setUp(const QVector3D &v);
    void lookAt(const QVector3D &postion, const QVector3D &direction, const QVector3D &up);
    void calcViewMatrix(QMatrix4x4 &v) const;
    void calcProjectMatrix(QMatrix4x4 &v) const;

signals:
    void rectChanged(void);
    void nearPlaneChanged(void);
    void farPlaneChanged(void);
    void fovChanged(void);
    void positionChanged(void);
    void directionChanged(void);
    void upChanged(void);

private:
    QRectF m_rect;
    float m_nearPlane;
    float m_farPlane;
    float m_fov;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
};

VOLCANO_INLINE const QRectF &Camera::rect(void) const
{
    return m_rect;
}

VOLCANO_INLINE void Camera::setRect(const QRectF &v)
{
    if (m_rect != v)
    {
        m_rect = v.normalized();
        rectChanged();
    }
}

VOLCANO_INLINE float Camera::nearPlane(void) const
{
    return m_nearPlane;
}

VOLCANO_INLINE void Camera::setNearPlane(float v)
{
    if (m_nearPlane != v)
    {
        m_nearPlane = v;
        nearPlaneChanged();
    }
}

VOLCANO_INLINE float Camera::farPlane(void) const
{
    return m_farPlane;
}

VOLCANO_INLINE void Camera::setFarPlane(float v)
{
    if (m_farPlane != v)
    {
        m_farPlane = v;
        farPlaneChanged();
    }
}

VOLCANO_INLINE float Camera::fov(void) const
{
    return m_fov;
}

VOLCANO_INLINE void Camera::setFov(float v)
{
    if (m_fov != v)
    {
        m_fov = v;
        fovChanged();
    }
}

VOLCANO_INLINE const QVector3D &Camera::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void Camera::setPosition(const QVector3D &v)
{
    if (m_position != v)
    {
        m_position= v;
        positionChanged();
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
        directionChanged();
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
        upChanged();
    }
}

VOLCANO_INLINE void Camera::lookAt(const QVector3D &position, const QVector3D &direction, const QVector3D &up)
{
    setPosition(position);
    setDirection(direction);
    setUp(up);
}

VOLCANO_INLINE void Camera::calcViewMatrix(QMatrix4x4 &v) const
{
    v.lookAt(m_position, m_position + m_direction, m_up);
}

VOLCANO_INLINE void Camera::calcProjectMatrix(QMatrix4x4 &v) const
{
    if (m_fov > 0.01f)
        v.perspective(m_fov, m_rect.width() / m_rect.height(), m_nearPlane, m_farPlane);
    else
        v.ortho(m_rect);
}

VOLCANO_END

#endif // VOLCANO_CAMERA_HPP
