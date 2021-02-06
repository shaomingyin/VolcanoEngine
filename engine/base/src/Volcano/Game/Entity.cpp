//
//
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

Entity::Entity(QObject *parent):
    QObject(parent),
    m_position(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f)
{
}

Entity::~Entity(void)
{
}

VOLCANO_GAME_END
