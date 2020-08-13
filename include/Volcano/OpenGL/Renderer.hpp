//
//
#ifndef VOLCANO_OPENGL_RENDERER_HPP
#define VOLCANO_OPENGL_RENDERER_HPP

#include <QSize>
#include <QVector3D>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/View.hpp>

VOLCANO_OPENGL_BEGIN

class VOLCANO_API Renderer
{
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(void);
    const QSize &size(void) const;
    void resize(const QSize &v);
    void resize(int width, int height);
    void render(const View &view);

private:
    void onSizeChanged(void);

private:
    Functions *m_gl;
    QSize m_size;
};

VOLCANO_INLINE const QSize &Renderer::size(void) const
{
    return m_size;
}

VOLCANO_INLINE void Renderer::resize(const QSize &v)
{
    if (m_size != v)
    {
        m_size = v;
        onSizeChanged();
    }
}

VOLCANO_INLINE void Renderer::resize(int width, int height)
{
    resize(QSize(width, height));
}

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_RENDERER_HPP
