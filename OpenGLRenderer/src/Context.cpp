//
//
#include <Volcano/OpenGLRenderer/Context.h>

VOLCANO_OPENGLRENDERER_BEGIN

Context::Context(Scene *scene):
    Renderer(scene)
{
}

Context::~Context(void)
{
}

bool Context::init(void)
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    if (context == nullptr)
        return false;

    m_gl = context->versionFunctions<OpenGLFunctions>();
    if (m_gl == nullptr)
        return false;

    return true;
}

bool Context::beginFrame(void)
{
    return true;
}

void Context::endFrame(void)
{
    Q_ASSERT(m_gl != nullptr);
}

void Context::setViewport(int x, int y, int width, int height)
{

}

void Context::setViewMatrix(const QMatrix4x4 &v)
{

}

void Context::setProjectMatrix(const QMatrix4x4 &v)
{

}

VOLCANO_OPENGLRENDERER_END
