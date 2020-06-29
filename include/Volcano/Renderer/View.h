//
//
#ifndef VOLCANO_RENDERER_VIEW_H
#define VOLCANO_RENDERER_VIEW_H

#include <QColor>
#include <QMatrix4x4>

#include <Volcano/Node.h>
#include <Volcano/Renderer/Common.h>

VOLCANO_RENDERER_BEGIN

class View
{
public:
    View();
    virtual ~View(void);

public:
    void reset(void);
    bool clear(void) const;
    void setClear(bool v);
    const QColor &clearColor(void) const;
    void setClearColor(const QColor &v);
    const QMatrix4x4 &viewMatrix(void) const;
    void setViewMatrix(const QMatrix4x4 &v);
    const QMatrix4x4 &projectMatrix(void) const;
    void setProjectMatrix(const QMatrix4x4 &v);

private:
    bool m_clear;
    QColor m_clearColor;
    QRect m_viewport;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectMatrix;
};

VOLCANO_INLINE bool View::clear(void) const
{
    return m_clear;
}

VOLCANO_INLINE void View::setClear(bool v)
{
    m_clear = v;
}

VOLCANO_INLINE const QColor &View::clearColor(void) const
{
    return m_clearColor;
}

VOLCANO_INLINE void View::setClearColor(const QColor &v)
{
    m_clearColor = v;
}

VOLCANO_INLINE const QMatrix4x4 &View::viewMatrix(void) const
{
    return m_viewMatrix;
}

VOLCANO_INLINE void View::setViewMatrix(const QMatrix4x4 &v)
{
    m_viewMatrix = v;
}

VOLCANO_INLINE const QMatrix4x4 &View::projectMatrix(void) const
{
    return m_projectMatrix;
}

VOLCANO_INLINE void View::setProjectMatrix(const QMatrix4x4 &v)
{
    m_projectMatrix = v;
}

VOLCANO_RENDERER_END

#endif // VOLCANO_RENDERER_VIEW_H
