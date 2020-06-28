//
//
#ifndef VOLCANO_RENDERER_VIEW_H
#define VOLCANO_RENDERER_VIEW_H

#include <QRect>
#include <QMatrix4x4>

#include <Volcano/Node.h>
#include <Volcano/Renderer/Common.h>

VOLCANO_RENDERER_BEGIN

class VOLCANO_API View
{
public:
    View();
    virtual ~View(void);

public:
    void clear(void);
    const QRect &viewport(void) const;
    void setViewport(const QRect &v);
    const QMatrix4x4 &viewMatrix(void) const;
    void setViewMatrix(const QMatrix4x4 &v);
    const QMatrix4x4 &projectMatrix(void) const;
    void setProjectMatrix(const QMatrix4x4 &v);

private:
    QRect m_viewport;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectMatrix;
};

VOLCANO_INLINE const QRect &View::viewport(void) const
{
    return m_viewport;
}

VOLCANO_INLINE void View::setViewport(const QRect &v)
{
    m_viewport = v.normalized();
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
