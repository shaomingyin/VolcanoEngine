//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent)
    : Object(parent)
    , camera_(nullptr) {
}

World::~World(void) {
}

Camera* World::camera() {
    return camera_;
}

void World::setCamera(Camera* p) {
    if (camera_ != p) {
        camera_ = p;
        emit cameraChanged(p);
    }
}

const SceneList &World::sceneList(void) const {
    return sceneList_;
}

void World::appendScene(Scene* scene) {
    sceneList_.append(scene);
    emit sceneAdded(scene);
}

Scene* World::sceneAt(qsizetype index) {
    return sceneList_.at(index);
}

void World::clearScenes(void) {
    for (Scene* scene: sceneList_)
        emit sceneRemoved(scene);
    sceneList_.clear();
}

qsizetype World::sceneCount(void) const {
    return sceneList_.count();
}

void World::replaceScene(qsizetype index, Scene* scene) {
    if (index < sceneList_.count()) {
        emit sceneRemoved(sceneList_.at(index));
        sceneList_.replace(index, scene);
        emit sceneAdded(scene);
    }
}

void World::removeLastScene(void) {
    if (!sceneList_.isEmpty()) {
        emit sceneRemoved(sceneList_.last());
        sceneList_.removeLast();
    }
}

VOLCANO_GAME_END
