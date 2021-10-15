//
//
#ifndef VOLCANO_GRAPHICS_CAMERARARENDERER_HPP
#define VOLCANO_GRAPHICS_CAMERARARENDERER_HPP

#include <QColor>
#include <QVector3D>
#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class CameraRenderer: public QQuickFramebufferObject::Renderer {
public:
    CameraRenderer(void);
    ~CameraRenderer(void) override;

public:
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;

private:
    bool init(void);
    void beginFrame(void);
    void endFrame(void);

private:
    OpenGLFunctions *m_gl;
    bool m_clearEnabled;
    QColor m_clearColor;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERARARENDERER_HPP
