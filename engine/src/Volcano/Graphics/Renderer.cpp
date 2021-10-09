//
//
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(QObject *parent):
    QObject(parent)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(void)
{
    // TODO init resources...

    return true;
}

void Renderer::resize(int width, int height)
{
}

void Renderer::beginFrame(void)
{
}

void Renderer::endFrame(void)
{
}

VOLCANO_GRAPHICS_END
