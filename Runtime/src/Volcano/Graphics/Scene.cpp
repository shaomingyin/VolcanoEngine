//
//
#include <Volcano/Graphics/Scene.hpp>

VOLCANO_GRAPHICS_BEGIN

Scene::Scene(Context &context, Game::Scene* gameScene, QObject *parent)
    : QObject(parent)
    , context_(context)
    , gameScene_(gameScene) {
    Q_ASSERT(gameScene_ != nullptr);
    auto& gameEntityList = gameScene_->entityList();
    for (auto& gameEntity: gameEntityList) {
        onGameEntityAdded(gameEntity);
    }
    connect(gameScene_, &Game::Scene::entityAdded, this, &Scene::onGameEntityAdded);
    connect(gameScene_, &Game::Scene::entityRemoved, this, &Scene::onGameEntityRemoved);
}

Scene::~Scene(void) {
}

Context &Scene::context(void) {
    return context_;
}

const Context &Scene::context(void) const {
    return context_;
}

Game::Scene* Scene::gameScene() {
    return gameScene_;
}

const Game::Scene* Scene::gameScene() const {
    return gameScene_;
}

void Scene::onGameEntityAdded(Game::Entity* p) {
}

void Scene::onGameEntityRemoved(Game::Entity* p) {
}

VOLCANO_GRAPHICS_END
