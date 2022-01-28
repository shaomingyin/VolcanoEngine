//
//
#include <Volcano/Game/Collision.hpp>

VOLCANO_GAME_BEGIN

Collision::Collision(QObject *parent):
    Component(parent),
    m_shape(nullptr)
{
}

Collision::~Collision(void)
{

}

Shape *Collision::shape(void)
{
    return m_shape;
}

void Collision::setShape(Shape *p)
{
    if (m_shape == p)
        return;

    // TODO physics service.

    m_shape = p;
    emit shapeChanged(p);
}

VOLCANO_GAME_END
