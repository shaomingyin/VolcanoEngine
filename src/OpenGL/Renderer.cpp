//
//
#include <Volcano/OpenGL/Renderer.hpp>

VOLCANO_OPENGL_BEGIN

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

    QOpenGLContext *context = QOpenGLContext::currentContext();
    Q_ASSERT(context != nullptr);

    m_gl = context->versionFunctions<Functions>();
    if (m_gl == nullptr)
        return false;

    qInfo() << "OpenGL:" << (const char *)m_gl->glGetString(GL_VERSION);

    return true;
}

void Renderer::render(const View &view)
{
    Q_ASSERT(m_gl != nullptr);

    if (view.clearEnabled())
    {
        const QColor &clearColor = view.clearColor();
        m_gl->glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
        m_gl->glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Renderer::onSizeChanged(void)
{
    Q_ASSERT(m_gl != nullptr);
}


VOLCANO_OPENGL_END
