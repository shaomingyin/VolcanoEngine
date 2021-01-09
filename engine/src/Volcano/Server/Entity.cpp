//
//
#include <Volcano/Server/Entity.hpp>

VOLCANO_SERVER_BEGIN

Entity::Entity(void):
    m_position(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f)
{
}

Entity::~Entity(void)
{
}

VOLCANO_SERVER_END
