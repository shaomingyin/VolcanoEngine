//
//
#ifndef VOLCANO_GRAPHICS_WORLD_HPP
#define VOLCANO_GRAPHICS_WORLD_HPP

#include <QList>
#include <QObject>

#include <Volcano/Game/WorldBase.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/Entity.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Graphics/Entity.hpp>

VOLCANO_GRAPHICS_BEGIN

class World: public QObject {
    Q_OBJECT
    Q_PROPERTY(Game::WorldBase *gameWorld READ gameWorld WRITE setGameWorld NOTIFY gameWorldChanged)

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    void reset(void);
    Game::WorldBase *gameWorld(void);
    void setGameWorld(Game::WorldBase *p);
    void buildView(View *view, const Camera &cam);

private:
    void onGameLightAdded(Game::Light *p);
    void onGameLightRemoved(Game::Light *p);
    void onGameDirectionalLightAdded(Game::DirectionalLight *p);
    void onGameDirectionalLightRemoved(Game::DirectionalLight *p);
    void onGameDirectionalLightEnabledChanged(Game::DirectionalLight *p, bool isEnabled);
    void onGamePointLightAdded(Game::PointLight *p);
    void onGamePointLightRemoved(Game::PointLight *p);
    void onGameSpotLightAdded(Game::SpotLight *p);
    void onGameSpotLightRemoved(Game::SpotLight *p);
    void onGameEntityAdded(Game::Entity *p);
    void onGameEntityRemoved(Game::Entity *p);

private slots:
    void onGameObjectAdded(Game::Object *p);
    void onGameObjectRemoved(Game::Object *p);

signals:
    void gameWorldChanged(Game::WorldBase *p);

private:
    using EntityList = QList<Entity *>;
    using GameDirectionalLightList = QList<Game::DirectionalLight *>;
    using GamePointLightList = QList<Game::PointLight *>;
    using GameSpotLightList = QList<Game::SpotLight *>;

    Game::WorldBase *m_gameWorld;
    GameDirectionalLightList m_gameDirectionalLightList;
    GamePointLightList m_gamePointLightList;
    GameSpotLightList m_gameSpotLightList;
    EntityList m_entityList;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WORLD_HPP
