//
//
#ifndef VOLCANO_VIEW_HPP
#define VOLCANO_VIEW_HPP

#include <QRectF>
#include <QVector3D>

#include <Volcano/Common.hpp>
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

class VOLCANO_API View
{
public:
    View(void);
    virtual ~View(void);

public:
    float fov(void) const;
    void setFov(float v);
    const QRectF &rect(void) const;
    void setRect(const QRectF &r);
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &r);
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &r);
    const QVector3D &up(void) const;
    void setUp(const QVector3D &r);
    Light &ambientLight(void);
    const Light &ambientLight(void) const;

private:
    float m_fov;
    QRectF m_rect;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    Light m_ambientLight;
};

VOLCANO_INLINE float View::fov(void) const
{
    return m_fov;
}

VOLCANO_INLINE void View::setFov(float v)
{
    m_fov = v;
}

VOLCANO_INLINE const QRectF &View::rect(void) const
{
    return m_rect;
}

VOLCANO_INLINE void View::setRect(const QRectF &r)
{
    m_rect = r.normalized();
}

VOLCANO_INLINE const QVector3D &View::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void View::setPosition(const QVector3D &r)
{
    m_position = r;
}

VOLCANO_INLINE const QVector3D &View::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void View::setDirection(const QVector3D &r)
{
    m_direction = r.normalized();
}

VOLCANO_INLINE const QVector3D &View::up(void) const
{
    return m_up;
}

VOLCANO_INLINE void View::setUp(const QVector3D &r)
{
    m_up = r.normalized();
}

VOLCANO_END

#endif // VOLCANO_VIEW_HPP
