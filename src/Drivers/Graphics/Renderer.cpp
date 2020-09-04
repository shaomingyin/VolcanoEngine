//
//
#include <Volcano/Drivers/Graphics/Renderer.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

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

bool Renderer::initPrograms(void)
{
    return true;
}

VOLCANO_DRIVERS_GRAPHICS_END
