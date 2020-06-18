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
    void update(float elapsed) override;
    void render(void) override;

private:
    OpenGLFunctions *m_gl;
};

VOLCANO_OPENGLRENDERER_END

#endif // VOLCANO_OPENGLRENDERER_CONTEXT_H
