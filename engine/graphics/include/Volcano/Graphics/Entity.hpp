//
//
#ifndef VOLCANO_GRAPHICS_ENTITY_HPP
#define VOLCANO_GRAPHICS_ENTITY_HPP

#include <QList>
#include <QMetaObject>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/VisibleSet.hpp>

VOLCANO_GRAPHICS_BEGIN

class Entity {
public:
    Entity(Game::Entity *gameEntity);
    Entity(Entity &&that);
    Entity(const Entity &that);
    virtual ~Entity(void);

public:
    void synchronize(void);
    void addToVS(VisibleSet &vs);
    Entity &operator=(Entity &&that);
    Entity &operator=(const Entity &that);

private:
    void init(void);
    void release(void);
    void onGameComponentAdded(Game::Component *gameComponent);
    void onGameComponentRemoved(Game::Component *gameComponent);

private:
    using MeshList = QList<Mesh>;

private:
    Game::Entity *m_gameEntity;
    QMetaObject::Connection m_gameComponentAddedConnection;
    QMetaObject::Connection m_gameComponentRemovedConnection;
    MeshList m_meshList;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_ENTITY_HPP
