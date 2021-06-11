//
//
#include <Volcano/UI/WorldRenderer.hpp>
#include <Volcano/UI/WorldView.hpp>

VOLCANO_UI_BEGIN

WorldView::WorldView(QQuickItem *parent):
    QQuickFramebufferObject(parent)
{
}

WorldView::~WorldView(void)
{
}

QQuickFramebufferObject::Renderer *WorldView::createRenderer(void) const
{
    m_renderer = new WorldRenderer();

    return m_renderer;
}

VOLCANO_UI_END
