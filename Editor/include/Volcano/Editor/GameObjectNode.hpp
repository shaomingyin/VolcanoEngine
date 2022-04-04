//
//
#ifndef VOLCANO_EDITOR_GAMEOBJECTNODE_HPP
#define VOLCANO_EDITOR_GAMEOBJECTNODE_HPP

#include <projectexplorer/projectnodes.h>

#include <Volcano/Game/Object.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class GameObjectNode: public ProjectExplorer::Node {
public:
    GameObjectNode(Game::Object *obj);
    ~GameObjectNode(void) override;

public:
    Game::Object *gameObject(void);

private:
    Game::Object *m_gameObject;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_GAMEOBJECTNODE_HPP
