//
//
#include <Volcano/World.hpp>

VOLCANO_BEGIN

World::World(Node *parent):
    Node(parent),
    m_gravity(0, -9.8, 0)
{
}

World::~World(void)
{
}

void World::setGravity(const QVector3D &v)
{
    if (m_gravity != v)
    {
        m_gravity = v;
        gravityChanged(v);
    }
}


void World::setViewport(Camera *p)
{
    if (m_viewport != p)
    {
        m_viewport = p;
        viewportChanged(p);
    }
}

void World::setFocus(Node *p)
{
    if (m_focus != p)
    {
        m_focus = p;
        focusChanged(p);
    }
}

void World::onUpdate(float elapsed)
{

}

void World::onDraw(OpenGL::View &view)
{
    if (Q_UNLIKELY(m_viewport == nullptr))
        return;

    // TODO check visibility for 'm_viewport'.

    view.setPosition(m_viewport->position());
    view.setDirection(m_viewport->direction());
    view.setUp(m_viewport->up());
}

VOLCANO_END
