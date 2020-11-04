//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Memory.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer
{
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(const QSize size);
    bool init(int width, int height);
    bool isInit(void) const;
    const QSize &size(void) const;
    void resize(const QSize &size);
    void resize(int width, int height);
    void render(const View &v);

private:
    bool initPrograms(void);

private:
    OpenGLFunctions *m_gl;
    QOpenGLShaderProgram m_program;
    QSize m_size;
    QOpenGLFramebufferObject m_fb;
    Memory m_memory;
};

VOLCANO_INLINE bool Renderer::init(int width, int height)
{
    return init(QSize(width, height));
}

VOLCANO_INLINE bool Renderer::isInit(void) const
{
    return (m_gl != nullptr);
}

VOLCANO_INLINE const QSize &Renderer::size(void) const
{
    return m_size;
}

VOLCANO_INLINE void Renderer::resize(int width, int height)
{
    resize(QSize(width, height));
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
