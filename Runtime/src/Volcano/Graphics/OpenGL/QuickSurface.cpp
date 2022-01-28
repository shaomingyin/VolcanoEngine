//
//
#include <memory>

#include <Volcano/Graphics/OpenGL/QuickSurface.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

QuickSurface::QuickSurface(QObject *parent):
    Volcano::Graphics::QuickSurface(parent)
{
}

QuickSurface::~QuickSurface(void)
{
}

QQuickGraphicsDevice *QuickSurface::createGraphicsDevice(void)
{
    return nullptr;
}

QQuickRenderTarget *QuickSurface::createRenderTarget(const QSize &size, int sampleCount)
{
    return nullptr;
}

VOLCANO_GRAPHICS_OPENGL_END
