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

bool Renderer::init(const QSize size)
{
    Q_ASSERT(m_gl == nullptr);

    if (!QOpenGLShaderProgram::hasOpenGLShaderPrograms())
        return false;

    OpenGLFunctions *gl = glFunctions();
    if (gl == nullptr)
        return false;

    if (!initPrograms())
        return false;

    resize(size);

    m_gl = gl;

    return true;
}

void Renderer::resize(const QSize &size)
{
    if (m_size == size)
        return;

    // TODO
}

void Renderer::render(const View &view)
{
    Q_ASSERT(m_gl != nullptr);

    if (Q_UNLIKELY(m_size.isEmpty()))
        return;
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
