//
//
#include <Volcano/Entity.hpp>

VOLCANO_BEGIN

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

VOLCANO_END
