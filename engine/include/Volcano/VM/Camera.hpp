//
//
#ifndef VOLCANO_VM_CAMERA_HPP
#define VOLCANO_VM_CAMERA_HPP

#include <Volcano/Graphics/View.hpp>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Object.hpp>

VOLCANO_VM_BEGIN

class Camera: public Object {
public:
    Camera(void);
    ~Camera(void) override;

public:
    void applyToView(Graphics::View &view) override;
    const glm::vec3 &position(void) const;
    void setPosition(const glm::vec3 &r);
    const glm::vec3 &direction(void) const;
    void setDirection(const glm::vec3 &r);
    const glm::vec3 &up(void) const;
    void setUp(const glm::vec3 &r);
    const RectF &rect(void) const;
    void setRect(const RectF &r);

private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;
    RectF m_rect;
};

VOLCANO_INLINE const glm::vec3 &Camera::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void Camera::setPosition(const glm::vec3 &r)
{
    VOLCANO_ASSERT(glm::epsilonEqual(glm::length(r), 1.0f, 0.001f));

    m_position = r;
}

VOLCANO_INLINE const glm::vec3 &Camera::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void Camera::setDirection(const glm::vec3 &r)
{
    VOLCANO_ASSERT(glm::epsilonEqual(glm::length(r), 1.0f, 0.001f));

    m_direction = r;
}

VOLCANO_INLINE const glm::vec3 &Camera::up(void) const
{
    return m_up;
}

VOLCANO_INLINE void Camera::setUp(const glm::vec3 &r)
{
    VOLCANO_ASSERT(glm::epsilonEqual(glm::length(r), 1.0f, 0.001f));

    m_up = r;
}

VOLCANO_INLINE const RectF &Camera::rect(void) const
{
    return m_rect;
}

VOLCANO_INLINE void Camera::setRect(const RectF &r)
{
    m_rect = r;
}

VOLCANO_VM_END

#endif // VOLCANO_VM_CAMERA_HPP
