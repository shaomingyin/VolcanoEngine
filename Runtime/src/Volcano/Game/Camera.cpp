//
//
#include <Volcano/Game/Camera.hpp>

VOLCANO_GAME_BEGIN

Camera::Camera(QObject *parent):
    Actor(parent),
    m_position(0.0f, 0.0f, 0.0f),
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_fov(90),
    m_nearPlane(0.1f),
    m_farPlane(100.0f)
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
    if (!qFuzzyCompare(m_position, v)) {
        m_position = v;
        emit positionChanged(v);
    }
}

const QVector3D &Camera::direction(void) const
{
    return m_direction;
}

void Camera::setDirection(const QVector3D &v)
{
    auto nv = v.normalized();
    if (!qFuzzyCompare(m_direction, nv)) {
        m_direction = nv;
        emit directionChanged(nv);
    }
}

const QVector3D &Camera::up(void) const
{
    return m_up;
}

void Camera::setUp(const QVector3D &v)
{
    auto nv = v.normalized();
    if (!qFuzzyCompare(m_up, nv)) {
        m_up = nv;
        emit upChanged(nv);
    }
}

float Camera::fov(void) const
{
    return m_fov;
}

void Camera::setFov(float v)
{
    if (v < 0.1f)
        return;

    if (!qFuzzyCompare(m_fov, v)) {
        m_fov = v;
        emit fovChanged(v);
    }
}

float Camera::nearPlane(void) const
{
    return m_nearPlane;
}

void Camera::setNearPlane(float v)
{
    if (v < 0.01f)
        return;

    if (v > m_farPlane)
        v = m_farPlane;

    if (!qFuzzyCompare(m_nearPlane, v)) {
        m_nearPlane = v;
        emit nearPlaneChanged(v);
    }
}

float Camera::farPlane(void) const
{
    return m_farPlane;
}

void Camera::setFarPlane(float v)
{
    if (v < 0.01f)
        return;

    if (v < m_nearPlane)
        v = m_nearPlane;

    if (!qFuzzyCompare(m_farPlane, v)) {
        m_farPlane = v;
        emit farPlaneChanged(v);
    }
}

VOLCANO_GAME_END
