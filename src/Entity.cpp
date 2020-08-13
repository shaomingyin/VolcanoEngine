//
//
#include <Volcano/Entity.hpp>

VOLCANO_BEGIN

Entity::Entity(Node *parent):
	Node(parent)
{
}

Entity::~Entity(void)
{
}

void Entity::onTick(float elapsed)
{

}

VOLCANO_END
