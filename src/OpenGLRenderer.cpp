//
//
#include <QOpenGLContext>

#include <Volcano/OpenGLRenderer.hpp>

VOLCANO_BEGIN

OpenGLRenderer::OpenGLRenderer(void):
    m_gl(nullptr)
{
}

OpenGLRenderer::~OpenGLRenderer(void)
{
}

bool OpenGLRenderer::init(void)
{
    Q_ASSERT(m_gl == nullptr);

    m_gl = QOpenGLContext::currentContext()->versionFunctions<OpenGLFunctions>();
    if (m_gl == nullptr)
        return false;

    qInfo() << "OpenGL:" << (const char *)m_gl->glGetString(GL_VERSION);

    m_gl->glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    return true;
}

void OpenGLRenderer::reset(int width, int height)
{
}

void OpenGLRenderer::render(void)
{
    Q_ASSERT(m_gl != nullptr);

    m_gl->glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_END
