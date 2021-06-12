//
//
#include <Volcano/Client/WorldRenderer.hpp>
#include <Volcano/Client/WorldView.hpp>

VOLCANO_CLIENT_BEGIN

WorldView::WorldView(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_world(nullptr)
{
}

WorldView::~WorldView(void)
{
}

WorldView::Renderer *WorldView::createRenderer(void) const
{
    m_renderer = new WorldRenderer();

    return m_renderer;
}

Game::World *WorldView::world(void)
{
    return m_world;
}

void WorldView::setWorld(Game::World *world)
{
    if (m_world != world) {
        m_world = world;
        worldChanged(world);
    }
}

VOLCANO_CLIENT_END
