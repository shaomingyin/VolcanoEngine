//
//
#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Game/Collision.hpp>

VOLCANO_GAME_BEGIN

Collision::Collision(QObject *parent):
    Component(parent),
    m_shape(nullptr)
{
}

Collision::Collision(Context *context, QObject *parent):
    Component(context, parent),
    m_shape(nullptr)
{
}

Collision::~Collision(void)
{

}

QObject *Collision::shape(void)
{
    return m_shape;
}

void Collision::setShape(QObject *p)
{
    if (m_shape == p)
        return;

    // TODO

    m_shape = p;
    emit shapeChanged(p);
}

VOLCANO_GAME_END
