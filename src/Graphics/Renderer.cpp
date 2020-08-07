//
//
#include <QOpenGLContext>

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

    m_gl = QOpenGLContext::currentContext()->versionFunctions<OpenGLFunctions>();
    if (m_gl == nullptr)
        return false;

    qInfo() << "OpenGL:" << (const char *)m_gl->glGetString(GL_VERSION);

    return true;
}

void Renderer::reset(int width, int height)
{
}

void Renderer::render(void)
{
    Q_ASSERT(m_gl != nullptr);

    m_gl->glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    m_gl->glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_GRAPHICS_END
