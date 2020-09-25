//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <QOpenGLShaderProgram>

#include <Volcano/Snapshot.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Target.hpp>
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Renderer
{
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(void);
    void setTarget(Target &r);
    void render(const Snapshot &r);

private:
    bool initPrograms(void);

private:
    OpenGLFunctions *m_gl;
    QOpenGLShaderProgram m_program;
    Target *m_target;
};

VOLCANO_INLINE void Renderer::setTarget(Target &r)
{
    m_target = &r;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
