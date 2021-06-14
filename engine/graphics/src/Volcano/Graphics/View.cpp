//
//
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

View::View(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_world(nullptr)
{
}

View::~View(void)
{
}

View::Renderer *View::createRenderer(void) const
{
    m_renderer = new ::Volcano::Graphics::Renderer();

    return m_renderer;
}

World *View::world(void)
{
    return m_world;
}

void View::setWorld(World *world)
{
    if (m_world != world) {
        m_world = world;
        worldChanged(world);
    }
}

VOLCANO_GRAPHICS_END
