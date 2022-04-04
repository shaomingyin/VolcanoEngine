//
//
#include <Volcano/Editor/GameObjectNode.hpp>

VOLCANO_EDITOR_BEGIN

GameObjectNode::GameObjectNode(Game::Object *obj):
    m_gameObject(obj)
{
    Q_ASSERT(m_gameObject != nullptr);
}

GameObjectNode::~GameObjectNode(void)
{
}

Game::Object *GameObjectNode::gameObject(void)
{
    return m_gameObject;
}

VOLCANO_EDITOR_END
