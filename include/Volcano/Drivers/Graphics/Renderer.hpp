//
//
#ifndef VOLCANO_DRIVERS_GRAPHICS_RENDERER_HPP
#define VOLCANO_DRIVERS_GRAPHICS_RENDERER_HPP

#include <QSize>
#include <QVector3D>
#include <QOpenGLShaderProgram>

#include <Volcano/Drivers/Graphics/Common.hpp>
#include <Volcano/Drivers/Graphics/Target.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class VOLCANO_API Renderer
{
    Q_DISABLE_COPY(Renderer);

public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(void);

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

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRAPHICS_RENDERER_HPP
