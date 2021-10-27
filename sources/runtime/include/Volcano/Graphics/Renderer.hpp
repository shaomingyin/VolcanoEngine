//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <QColor>
#include <QVector3D>
#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public QQuickFramebufferObject::Renderer {
public:
    Renderer(void);
    ~Renderer(void) override;

public:
    bool init(void);
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;

private:

private:
    OpenGLFunctions *m_gl;
    bool m_clearEnabled;
    QColor m_clearColor;
    QVector3D m_viewPosition;
    QVector3D m_viewDirection;
    QVector3D m_viewUp;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
