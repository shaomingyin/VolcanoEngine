//
//
#ifndef VOLCANO_GRAPHICS_WORLD_HPP
#define VOLCANO_GRAPHICS_WORLD_HPP

#include <QList>

#include <Volcano/Game/World.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Context.hpp>
#include <Volcano/Graphics/Scene.hpp>

VOLCANO_GRAPHICS_BEGIN

class World: public QObject {
    Q_OBJECT

public:
    World(Context &context, Game::World* gameWorld, QObject *parent = nullptr);
    ~World(void) override;

public:
    Context &context(void);
    const Context &context(void) const;
    Game::World* gameWorld();
    const Game::World* gameWorld() const;

private slots:
    void onGameSceneAdded(Game::Scene* p);
    void onGameSceneRemoved(Game::Scene* p);

private:
    Context &context_;
    Game::World* gameWorld_;
    QList<Scene*> sceneList_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WORLD_HPP
