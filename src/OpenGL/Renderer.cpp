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

    Functions *gl = context->versionFunctions<Functions>();
    if (gl == nullptr)
        return false;

    qInfo() << "OpenGL:" << (const char *)gl->glGetString(GL_VERSION);

    if (!initPrograms())
        return false;

    m_gl = gl;

    return true;
}

void Renderer::render(const View &view, Target &target)
{
    Q_ASSERT(m_gl != nullptr);

    if (view.clearEnabled())
    {
        const QColor &clearColor = view.clearColor();
        m_gl->glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
        m_gl->glClear(GL_COLOR_BUFFER_BIT);
    }
}

bool Renderer::initPrograms(void)
{
    return true;
}

VOLCANO_OPENGL_END
