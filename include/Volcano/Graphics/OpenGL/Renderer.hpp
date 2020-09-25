//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_RENDERER_HPP
#define VOLCANO_GRAPHICS_OPENGL_RENDERER_HPP

#include <QOpenGLShaderProgram>

#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Graphics/OpenGL/Common.hpp>
#include <Volcano/Graphics/OpenGL/Target.hpp>
#include <Volcano/Graphics/OpenGL/Mesh.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class VOLCANO_API Renderer: public Volcano::Graphics::Renderer
{
public:
    Renderer(void);
    ~Renderer(void) override;

public:
    bool init(void) override;
    void setTarget(Target &r);
    //void render(const Snapshot &r);

private:
    bool initPrograms(void);

private:
    QOpenGLShaderProgram m_program;
    Functions *m_gl;
    Target *m_target;
};

VOLCANO_INLINE void Renderer::setTarget(Target &r)
{
    m_target = &r;
}

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_RENDERER_HPP
