//
//
#ifndef VOLCANO_OPENGLRENDERER_CONTEXT_H
#define VOLCANO_OPENGLRENDERER_CONTEXT_H

#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Renderer.h>
#include <Volcano/OpenGLRenderer/Common.h>

VOLCANO_OPENGLRENDERER_BEGIN

typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;

class VOLCANO_OPENGLRENDERER_API Context: public Renderer
{
public:
    Context(Scene *scene);
    ~Context(void) override;

public:
    bool init(void);

protected:
    bool beginFrame(void) override;
    void endFrame(void) override;
    void setViewport(int x, int y, int width, int height) override;
    void setViewMatrix(const QMatrix4x4 &v) override;
    void setProjectMatrix(const QMatrix4x4 &v) override;

private:
    OpenGLFunctions *m_gl;
};

VOLCANO_OPENGLRENDERER_END

#endif // VOLCANO_OPENGLRENDERER_CONTEXT_H
