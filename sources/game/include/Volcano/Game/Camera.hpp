//
//
#ifndef VOLCANO_GAME_CAMERA_HPP
#define VOLCANO_GAME_CAMERA_HPP

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Camera {
public:
    Camera(void);
    virtual ~Camera(void);

public:
    const Eigen::Vector3f &position(void) const;
    void setPosition(const Eigen::Vector3f &r);
    const Eigen::Vector3f &direction(void) const;
    void setDirection(const Eigen::Vector3f &r);
    const Eigen::Vector3f &up(void) const;
    void setUp(const Eigen::Vector3f &r);
    const Eigen::AlignedBox2f &rect(void) const;
    void setRect(const Eigen::AlignedBox2f &r);
    virtual void update(float elapsed) {};

private:
    Eigen::Vector3f m_position;
    Eigen::Vector3f m_direction;
    Eigen::Vector3f m_up;
    Eigen::AlignedBox2f m_rect;
};

VOLCANO_INLINE const Eigen::Vector3f &Camera::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void Camera::setPosition(const Eigen::Vector3f &r)
{
    m_position = r;
}

VOLCANO_INLINE const Eigen::Vector3f &Camera::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void Camera::setDirection(const Eigen::Vector3f &r)
{
    m_direction = r;
}

VOLCANO_INLINE const Eigen::Vector3f &Camera::up(void) const
{
    return m_up;
}

VOLCANO_INLINE void Camera::setUp(const Eigen::Vector3f &r)
{
    m_up = r;
}

VOLCANO_INLINE const Eigen::AlignedBox2f &Camera::rect(void) const
{
    return m_rect;
}

VOLCANO_INLINE void Camera::setRect(const Eigen::AlignedBox2f &r)
{
    m_rect = r;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CAMERA_HPP
