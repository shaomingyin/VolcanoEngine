//
//
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

Entity::Entity(QObject *parent):
    QObject(parent)
{
}

Entity::~Entity(void)
{
}

void Entity::update(float elapsed)
{
}

VOLCANO_GAME_END
