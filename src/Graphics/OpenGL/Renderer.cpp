//
//
#include <QOpenGLShader>

#include <Volcano/Graphics/OpenGL/Renderer.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

Renderer::Renderer(void):
    m_gl(nullptr)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(void)
{
    Q_ASSERT(m_gl == nullptr);

    if (!QOpenGLShaderProgram::hasOpenGLShaderPrograms())
        return false;

    Functions *gl = functions();
    if (gl == nullptr)
        return false;

    if (!initPrograms())
        return false;

    m_gl = gl;

    return true;
}

Mesh *Renderer::createMesh(int vertexCount, int vertexIndexCount, Mesh::Type type)
{
    Mesh *p = new Mesh(m_memory);
    if (p == nullptr)
        return nullptr;

    if (!p->init(vertexCount, vertexIndexCount, type))
    {
        delete p;
        return nullptr;
    }

    return p;
}

Target *Renderer::createTarget(int width, int height)
{
    Target *p = new Target(width, height);
    if (p == nullptr)
        return nullptr;

    return nullptr;
}

void Renderer::beginFrame(void)
{

}

void Renderer::endFrame(void)
{

}

bool Renderer::initPrograms(void)
{
    if (m_program.programId() < 1)
    {
        if (!m_program.create())
            return false;
    }
    else
    {
        m_program.removeAllShaders();
    }

    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, ""))
        return false;

    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, ""))
        return false;

    if (!m_program.link())
        return false;

    return true;
}

VOLCANO_GRAPHICS_OPENGL_END
