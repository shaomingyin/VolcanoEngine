//
//
#include <QOpenGLContext>

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
    if (context == nullptr)
        return false;

    m_gl = context->versionFunctions<Functions>();
    if (m_gl == nullptr)
        return false;

    return true;
}

void Renderer::render(const Snapshot &r)
{
    Q_ASSERT(m_gl != nullptr);

    const View &view = r.view();
}

VOLCANO_OPENGL_END
