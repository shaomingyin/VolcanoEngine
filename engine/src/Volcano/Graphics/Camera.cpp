//
//
#include <Volcano/Graphics/Camera.hpp>

VOLCANO_GRAPHICS_BEGIN

Camera::Camera(QObject *parent):
    QObject(parent),
    m_worldCache(nullptr)
{
}

Camera::~Camera(void)
{
}

const QVector3D &Camera::position(void) const
{
    return m_position;
}

void Camera::setPosition(const QVector3D &v)
{
    if (m_position != v) {
        m_position = v.normalized();
        emit positionChanged(m_position);
    }
}

const QVector3D &Camera::direction(void) const
{
    return m_direction;
}

void Camera::setDirection(const QVector3D &v)
{
    if (m_direction != v) {
        m_direction = v.normalized();
        emit directionChanged(m_direction);
    }
}

const QVector3D &Camera::up(void) const
{
    return m_up;
}

void Camera::setUp(const QVector3D &v)
{
    if (m_up != v) {
        m_up = v.normalized();
        emit upChanged(m_up);
    }
}

const QRectF &Camera::rect(void) const
{
    return m_rect;
}

void Camera::setRect(const QRectF &v)
{
    if (m_rect != v) {
        m_rect = v.normalized();
        emit rectChanged(m_rect);
    }
}

float Camera::nearPlane(void) const
{
    return m_nearPlane;
}

void Camera::setNearPlane(float v)
{
    if (!qFuzzyCompare(m_nearPlane, v)) {
        m_nearPlane = v;
        emit nearPlaneChanged(v);
    }
}

float Camera::farPlane() const
{
    return m_farPlane;
}

void Camera::setFarPlane(float v)
{
    if (!qFuzzyCompare(m_farPlane, v)) {
        m_farPlane = v;
        emit farPlaneChanged(v);
    }
}

WorldCache *Camera::worldCache(void)
{
    return m_worldCache;
}

void Camera::setWorldCache(WorldCache *worldCache)
{
    if (m_worldCache != worldCache) {
        m_worldCache = worldCache;
        emit worldCacheChanged(worldCache);
    }
}

void Camera::calcProjectMatrix(QMatrix4x4 &out)
{
    out.ortho(m_rect.left(), m_rect.right(), m_rect.top(), m_rect.bottom(), m_nearPlane, m_farPlane);
}

VOLCANO_GRAPHICS_END
