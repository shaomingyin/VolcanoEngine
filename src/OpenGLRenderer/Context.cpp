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

void Context::update(float elapsed)
{
}

void Context::render(void)
{
    Q_ASSERT(m_gl != nullptr);
}

VOLCANO_OPENGLRENDERER_END
