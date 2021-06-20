//
//
#ifndef VOLCANO_OPENGL_RENDERER_HPP
#define VOLCANO_OPENGL_RENDERER_HPP

#include <QSize>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>

#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

class Renderer {
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(int width, int height);
    const QSize &size(void);
    void resize(const QSize &v);
    void beginFrame(void);
    void endFrame(void);

private:
    void updateGBuffer(void);
    void releaseGBuffer(void);

private:
    Functions *m_gl;
    QSize m_size;
    QOpenGLFramebufferObject *m_gBuffer;
    QOpenGLTexture *m_positionTexture;
    QOpenGLTexture *m_colorTexture;
    QOpenGLContext *m_normalTexture;
    QOpenGLContext *m_depthTexture;
    QOpenGLContext *m_texCoordTexture;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_RENDERER_HPP
