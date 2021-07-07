//
//
#ifndef VOLCANO_GRAPHICS_ENTITY_HPP
#define VOLCANO_GRAPHICS_ENTITY_HPP

#include <QList>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>
#include <Volcano/Graphics/VisibleSet.hpp>

VOLCANO_GRAPHICS_BEGIN

class Entity {
public:
    Entity(Game::Entity *gameEntity);
    Entity(Entity &&that);
    virtual ~Entity(void);

public:
    void synchronize(void);
    void addToVS(VisibleSet &vs);

private:
    struct Mesh {
        Buffer *m_vertexBuffer;
        Buffer *m_vertexIndexBuffer;
    };

    using MeshList = QList<Mesh>;

    Game::Entity *m_gameEntity;
    MeshList m_meshList;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_ENTITY_HPP
