//
//
#include <memory>

#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(void)
{
}

Renderer::~Renderer(void)
{
}

void Renderer::render(void)
{
    QQuickOpenGLUtils::resetOpenGLState();
}

void Renderer::synchronize(QQuickFramebufferObject *item)
{
    Q_UNUSED(item);

}

VOLCANO_GRAPHICS_END
