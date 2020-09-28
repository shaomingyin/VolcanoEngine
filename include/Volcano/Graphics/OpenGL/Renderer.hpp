//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_RENDERER_HPP
#define VOLCANO_GRAPHICS_OPENGL_RENDERER_HPP

#include <QOpenGLShaderProgram>

#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Graphics/OpenGL/Common.hpp>
#include <Volcano/Graphics/OpenGL/Target.hpp>
#include <Volcano/Graphics/OpenGL/Memory.hpp>
#include <Volcano/Graphics/OpenGL/Mesh.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class VOLCANO_API Renderer: public Graphics::Renderer
{
public:
    Renderer(void);
    ~Renderer(void) override;

public:
    bool init(void) override;
    Mesh *createMesh(int vertexCount, int vertexIndexCount, Mesh::Type type) override;
    Target *createTarget(int width, int height) override;
    void beginFrame(void) override;
    void endFrame(void) override;

private:
    bool initPrograms(void);

private:
    QOpenGLShaderProgram m_program;
    Functions *m_gl;
    Memory m_memory;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_RENDERER_HPP
