//
//
#include <Volcano/OpenGL/Renderer.hpp>

VOLCANO_OPENGL_BEGIN

Renderer::Renderer(void):
    m_fns(currentFunctions())
{
    Q_ASSERT(m_fns != nullptr);
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(const QSize &size)
{
    m_size = size;

    return true;
}

const QSizeF &Renderer::size(void)
{
    return m_size;
}

void Renderer::setSize(const QSizeF &v)
{
    if (m_size != v) {
        m_size = v;
        emit sizeChanged(v);
    }
}

void Renderer::beginFrame(void)
{
}

void Renderer::endFrame(void)
{
    m_fns->glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    m_fns->glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_OPENGL_END
