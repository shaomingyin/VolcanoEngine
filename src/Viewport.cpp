//
//
#include <Volcano/Renderer.h>
#include <Volcano/Viewport.h>

VOLCANO_BEGIN

Viewport::Viewport(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_target(nullptr),
    m_camera(nullptr)
{
}

Viewport::~Viewport(void)
{
}

void Viewport::setTarget(Node *p)
{
    if (m_target != p)
    {
        m_target = p;
        targetChanged(p);
    }
}

void Viewport::setCamera(Camera *p)
{
    if (m_camera != p)
    {
        m_camera = p;
        cameraChanged(p);
    }
}

Viewport::Renderer *Viewport::createRenderer(void) const
{
    return (new Volcano::Renderer());
}

VOLCANO_END
