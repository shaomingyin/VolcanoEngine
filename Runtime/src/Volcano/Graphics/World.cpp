//
//
#include <Volcano/Graphics/World.hpp>

VOLCANO_GRAPHICS_BEGIN

World::World(Context &context, Game::World* gameWorld, QObject *parent)
    : QObject(parent)
    , context_(context)
    , gameWorld_(gameWorld) {
    Q_ASSERT(gameWorld_ != nullptr);
    auto& gameSceneList = gameWorld_->sceneList();
    for (Game::Scene* gameScene: gameSceneList) {
        onGameSceneAdded(gameScene);
    }
    connect(gameWorld_, &Game::World::sceneAdded, this, &World::onGameSceneAdded);
    connect(gameWorld_, &Game::World::sceneAdded, this, &World::onGameSceneRemoved);
}

World::~World(void) {
}

Context& World::context(void) {
    return context_;
}

const Context& World::context(void) const {
    return context_;
}

Game::World* World::gameWorld() {
    return gameWorld_;
}

const Game::World* World::gameWorld() const {
    return gameWorld_;
}

void World::onGameSceneAdded(Game::Scene* p) {
    sceneList_.append(new Scene(context_, p, this));
}

void World::onGameSceneRemoved(Game::Scene* p) {
    sceneList_.removeIf([p](Scene* scene) { return (scene->gameScene() == p); });
}

VOLCANO_GRAPHICS_END
