//
//
#ifndef VOLCANO_OPENGLRENDERER_HPP
#define VOLCANO_OPENGLRENDERER_HPP

#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API OpenGLRenderer
{
public:
    OpenGLRenderer(void);
    virtual ~OpenGLRenderer(void);

public:
    bool init(void);
    void reset(int width, int height);
    void render(void);

private:
    typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;

    OpenGLFunctions *m_gl;
    int m_width;
    int m_height;
};

VOLCANO_END

#endif // VOLCANO_OPENGLRENDERER_HPP
