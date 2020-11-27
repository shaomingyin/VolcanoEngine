//
//
#include <Volcano/VM/Entity.hpp>

VOLCANO_VM_BEGIN

Entity::Entity(void):
    m_position(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f)
{
}

Entity::~Entity(void)
{
}

void Entity::applyToView(Graphics::View &view)
{
}

VOLCANO_VM_END
