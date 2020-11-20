//
//
#ifndef VOLCANO_GRAPHICS_VIEW_HPP
#define VOLCANO_GRAPHICS_VIEW_HPP

#include <Volcano/Rect.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class View {
public:
    enum Mode {
        MODE_ORTHO = 0,
        MODE_PERPECTIVE
    };

public:
    View(void);
    virtual ~View(void);

public:
    void reset(void);
    float elapsed(void) const;
    void setElapsed(float v);
    Mode mode(void) const;
    void setMode(Mode v);
    const glm::vec3 &position(void) const;
    void setPosition(const glm::vec3 &r);
    const glm::vec3 &direction(void) const;
    void setDirection(const glm::vec3 &r);
    const glm::vec3 &up(void) const;
    void setUp(const glm::vec3 &r);
    const RectF &rect(void) const;
    void setRect(const RectF &r);
    float fov(void) const;
    void setFov(float v);

private:
    float m_elapsed;
    Mode m_mode;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;
    RectF m_rect;
    float m_fov;
};

VOLCANO_INLINE void View::reset(void)
{
}

VOLCANO_INLINE float View::elapsed(void) const
{
    return m_elapsed;
}

VOLCANO_INLINE void View::setElapsed(float v)
{
    m_elapsed = v;
}

VOLCANO_INLINE View::Mode View::mode(void) const
{
    return m_mode;
}

VOLCANO_INLINE void View::setMode(Mode v)
{
    m_mode = v;
}

VOLCANO_INLINE const glm::vec3 &View::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void View::setPosition(const glm::vec3 &r)
{
    m_position = r;
}

VOLCANO_INLINE const glm::vec3 &View::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void View::setDirection(const glm::vec3 &r)
{
    m_direction = r;
}

VOLCANO_INLINE const glm::vec3 &View::up(void) const
{
    return m_up;
}

VOLCANO_INLINE void View::setUp(const glm::vec3 &r)
{
    m_up = r;
}

VOLCANO_INLINE const RectF &View::rect(void) const
{
    return m_rect;
}

VOLCANO_INLINE void View::setRect(const RectF &r)
{
    m_rect = r.normalized();
}

VOLCANO_INLINE float View::fov(void) const
{
    VOLCANO_ASSERT(m_mode == MODE_PERPECTIVE);
    return m_fov;
}

VOLCANO_INLINE void View::setFov(float v)
{
    VOLCANO_ASSERT(m_mode == MODE_PERPECTIVE);
    m_fov = v;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
