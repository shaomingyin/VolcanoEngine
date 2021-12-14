//
//
#ifndef VOLCANO_GRAPHICS_ENTITY_HPP
#define VOLCANO_GRAPHICS_ENTITY_HPP

#include <QObject>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Material.hpp>
#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class Entity: public QObject {
    Q_OBJECT

public:
    Entity(Game::Entity *p, QObject *parent = nullptr);
    ~Entity(void) override;

public:
    Game::Entity *gameEntity(void);
    bool isValid(void) const;
    void buildView(View *view, const Camera &cam) const;

private:
    void onGameMaterialAdded(Game::Material *mat);
    void onGameMeshAdded(Game::Mesh *mesh);
    void onGameMaterialRemoved(Game::Material *p);
    void onGameMeshRemoved(Game::Mesh *p);

private slots:
    void onGameComponentAdded(Game::Component *comp);
    void onGameComponentRemoved(Game::Component *comp);

private:
    Game::Entity *m_gameEntity;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_ENTITY_HPP
