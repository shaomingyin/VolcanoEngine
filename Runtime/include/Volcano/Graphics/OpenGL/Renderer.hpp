//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_RENDERER_HPP
#define VOLCANO_GRAPHICS_OPENGL_RENDERER_HPP

#include <QSize>
#include <QColor>
#include <QVector3D>
#include <QOpenGLFramebufferObject>

#include <Volcano/Graphics/Service.hpp>
#include <Volcano/Graphics/OpenGL/Common.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class Renderer {
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(int width, int height);
    void resize(int width, int height);
    void addMesh(Mesh *p);
    void bindMaterial(Material *p);
    void beginFrame(void);
    void endFrame(void);

private:
    Functions *m_gl;
    QOpenGLFramebufferObject *m_fbo;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_RENDERER_HPP
