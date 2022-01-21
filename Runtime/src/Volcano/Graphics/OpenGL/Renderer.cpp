//
//
#include <memory>

#include <QThread>
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/OpenGL/Context.hpp>
#include <Volcano/Graphics/OpenGL/Renderer.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

Renderer::Renderer(void):
    m_gl(nullptr)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(int width, int height)
{
    auto context = Context::current();
    if (context == nullptr)
        return false;

    m_gl = context->functions();
    if (m_gl == nullptr)
        return false;

    m_gl->glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    return true;
}

void Renderer::resize(int width, int height)
{
}

void Renderer::addMesh(Mesh *p)
{
}

void Renderer::bindMaterial(Material *p)
{
}

void Renderer::beginFrame(void)
{
}

void Renderer::endFrame(void)
{
    m_gl->glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_GRAPHICS_OPENGL_END
