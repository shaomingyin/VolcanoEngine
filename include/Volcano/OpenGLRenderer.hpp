//
//
#ifndef VOLCANO_OPENGLRENDERER_HPP
#define VOLCANO_OPENGLRENDERER_HPP

#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class OpenGLRenderer
{
public:
    OpenGLRenderer(void);
    virtual ~OpenGLRenderer(void);

public:
    bool init(void);
    void beginFrame(void);
    void endFrame(void);

private:
    typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;

    OpenGLFunctions *m_gl;
};

VOLCANO_END

#endif // VOLCANO_OPENGLRENDERER_HPP
