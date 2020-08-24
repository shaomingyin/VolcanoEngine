//
//
#ifndef VOLCANO_OPENGL_VIEW_HPP
#define VOLCANO_OPENGL_VIEW_HPP

#include <QColor>
#include <QVector3D>

#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

class VOLCANO_API View
{
public:
    View(void);
    virtual ~View(void);

public:
    void reset(void);
    bool clearEnabled(void) const;
    void setClearEnabled(bool v);
    const QColor &clearColor(void) const;
    void setClearColor(const QColor &v);
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &v);
    const QVector3D &up(void) const;
    void setUp(const QVector3D &v);

private:
    bool m_clearEnabled;
    QColor m_clearColor;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
};

VOLCANO_INLINE void View::reset(void)
{
}

VOLCANO_INLINE bool View::clearEnabled(void) const
{
    return m_clearEnabled;
}

VOLCANO_INLINE void View::setClearEnabled(bool v)
{
    m_clearEnabled = v;
}

VOLCANO_INLINE const QColor &View::clearColor(void) const
{
    return m_clearColor;
}

VOLCANO_INLINE void View::setClearColor(const QColor &v)
{
    m_clearColor = v;
}

VOLCANO_INLINE const QVector3D &View::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void View::setPosition(const QVector3D &v)
{
    m_position = v.normalized();
}

VOLCANO_INLINE const QVector3D &View::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void View::setDirection(const QVector3D &v)
{
    m_direction = v.normalized();
}

VOLCANO_INLINE const QVector3D &View::up(void) const
{
    return m_up;
}

VOLCANO_INLINE void View::setUp(const QVector3D &v)
{
    m_up = v.normalized();
}

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_VIEW_HPP
