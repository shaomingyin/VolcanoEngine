//
//
#include <Volcano/Scene/Entity.hpp>

VOLCANO_SCENE_BEGIN

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

VOLCANO_SCENE_END
