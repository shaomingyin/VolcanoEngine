//
//
#include <Volcano/OpenGL/Renderer.hpp>

VOLCANO_OPENGL_BEGIN

Renderer::Renderer(void):
    m_funcs(nullptr),
    m_gBuffer(nullptr)
{
}

Renderer::~Renderer(void)
{
    if (m_gBuffer != nullptr)
        delete m_gBuffer;
}

bool Renderer::init(int width, int height)
{
    Q_ASSERT(m_funcs == nullptr);
    Q_ASSERT(m_gBuffer == nullptr);
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);

    m_funcs = currentFunctions();
    if (m_funcs == nullptr)
        return false;

    m_size.setWidth(width);
    m_size.setHeight(height);

    return true;
}

const QSize &Renderer::size(void)
{
    return m_size;
}

void Renderer::setSize(const QSize &v)
{
    if (m_size == v)
        return;

    m_size = v;

    // TODO resize gbuffer.
}

void Renderer::clear(void)
{
    Q_ASSERT(m_funcs != nullptr);

    m_funcs->glClear(GL_COLOR_BUFFER_BIT);

}

const QColor &Renderer::clearColor(void) const
{
    return m_clearColor;
}

void Renderer::setClearColor(const QColor &v)
{
    m_clearColor = v;
}

void Renderer::beginFrame(void)
{

}

void Renderer::endFrame(void)
{
}

void Renderer::loadIdenity(void)
{
    // m_translate
}

void Renderer::translate(const QVector3D &v)
{
}

void Renderer::scale(const QVector3D &v)
{
}

void Renderer::rotate(GLfloat angle, const QVector3D &axis)
{
}

VOLCANO_OPENGL_END
