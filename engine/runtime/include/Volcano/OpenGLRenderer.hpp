//
//
#ifndef VOLCANO_OPENGLRENDERER_HPP
#define VOLCANO_OPENGLRENDERER_HPP

#include <QSize>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>

#include <Volcano/OpenGLCommon.hpp>

VOLCANO_BEGIN

class OpenGLRenderer {
public:
    OpenGLRenderer(void);
    virtual ~OpenGLRenderer(void);

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
    OpenGLFunctions *m_gl;
    QSize m_size;
    QOpenGLFramebufferObject *m_gBuffer;
    QOpenGLTexture *m_positionTexture;
    QOpenGLTexture *m_colorTexture;
    QOpenGLContext *m_normalTexture;
    QOpenGLContext *m_depthTexture;
    QOpenGLContext *m_texCoordTexture;
};

VOLCANO_END

#endif // VOLCANO_OPENGLRENDERER_HPP
