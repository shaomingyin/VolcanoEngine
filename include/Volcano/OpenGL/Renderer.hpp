//
//
#ifndef VOLCANO_OPENGL_RENDERER_HPP
#define VOLCANO_OPENGL_RENDERER_HPP

#include <QSize>
#include <QVector3D>
#include <QOpenGLShaderProgram>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/View.hpp>
#include <Volcano/OpenGL/Target.hpp>

VOLCANO_OPENGL_BEGIN

class VOLCANO_API Renderer
{
    Q_DISABLE_COPY(Renderer);

public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(void);
    void render(const View &view, Target &target);

private:
    bool initPrograms(void);

protected:
    enum ProgramType
    {
        ProgramDefault = 0,

        ProgramMax
    };

private:
    Functions *m_gl;
    QOpenGLShaderProgram m_programs[ProgramMax];
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_RENDERER_HPP
