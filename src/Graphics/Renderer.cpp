//
//
#include <QOpenGLShader>

#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

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

    OpenGLFunctions *gl = glFunctions();
    if (gl == nullptr)
        return false;

    if (!initPrograms())
        return false;

    m_gl = gl;

    return true;
}

void Renderer::render(const Snapshot &r)
{
    Q_ASSERT(m_gl != nullptr);

    const View &view = r.view();
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

VOLCANO_GRAPHICS_END
