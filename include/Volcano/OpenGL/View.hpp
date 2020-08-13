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

private:
    bool m_clearEnabled;
    QColor m_clearColor;
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

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_VIEW_HPP
