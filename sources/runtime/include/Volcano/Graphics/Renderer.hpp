//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <QColor>
#include <QVector3D>
#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/DrawingSet.hpp>
#include <Volcano/Graphics/DebugDrawRenderer.hpp>

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
    DebugDrawRenderer m_debugDrawRenderer;
    bool m_clearEnabled;
    QColor m_clearColor;
    QVector3D m_viewPosition;
    QVector3D m_viewDirection;
    QVector3D m_viewUp;
    DrawingSet m_drawingSet;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
