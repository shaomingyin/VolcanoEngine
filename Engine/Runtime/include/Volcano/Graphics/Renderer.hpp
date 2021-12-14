//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <QSize>
#include <QColor>
#include <QVector3D>
#include <QOpenGLFramebufferObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public View {
public:
    Renderer(void);
    ~Renderer(void) override;

public:
    bool init(int width, int height);
    void reset(void) override;
    void resize(const QSize &v);
    void resize(int width, int height);
    void render(void);

private:
    QSize m_size;
    OpenGLFunctions *m_gl;
    QOpenGLFramebufferObject *m_fbo;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
