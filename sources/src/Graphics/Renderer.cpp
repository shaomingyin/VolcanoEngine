//
//
#include <QOpenGLShader>

#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(void):
    m_gl(nullptr),
    m_fb(nullptr)
{
}

Renderer::~Renderer(void)
{
    if (m_fb != nullptr)
    {
        // TODO
    }
}

bool Renderer::init(const QSize size)
{
    Q_ASSERT(m_gl == nullptr);

    if (!QOpenGLShaderProgram::hasOpenGLShaderPrograms())
        return false;

    OpenGLFunctions *gl = glFunctions();
    if (gl == nullptr)
        return false;

    //if (!initPrograms())
    //    return false;

    resize(size);

    m_gl = gl;

    return true;
}

void Renderer::shutdown(void)
{
    Q_ASSERT(m_gl != nullptr);

    // TODO
}

void Renderer::resize(const QSize &size)
{
    if (m_size == size)
        return;

    if (m_fb != nullptr)
    {

    }

    // TODO

    m_size = size;
}

void Renderer::render(const View &view)
{
    Q_ASSERT(m_gl != nullptr);

    if (Q_UNLIKELY(m_size.isEmpty()))
        return;

    m_gl->glClear(GL_COLOR_BUFFER_BIT);
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
