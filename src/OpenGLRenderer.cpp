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

    m_gl->glClearColor(0.3f, 0.8f, 0.3f, 0.0f);

    return true;
}

void OpenGLRenderer::beginFrame(void)
{
}

void OpenGLRenderer::endFrame(void)
{
    Q_ASSERT(m_gl != nullptr);

    m_gl->glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_END
