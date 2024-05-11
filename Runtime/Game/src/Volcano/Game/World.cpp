//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent)
    : Object(parent)
    , camera_(nullptr) {
}

World::World(Context& context, QObject *parent)
    : Object(context, parent)
    , camera_(nullptr) {
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

QQmlListProperty<Scene> World::qmlSceneList() {
    return { this, this,
        [](QQmlListProperty<Scene>* property, Scene* p) { reinterpret_cast<World*>(property->data)->appendScene(p); },
        [](QQmlListProperty<Scene>* property) { return reinterpret_cast<World*>(property->data)->sceneCount(); },
        [](QQmlListProperty<Scene>* property, qsizetype index) { return reinterpret_cast<World*>(property->data)->sceneAt(index); },
        [](QQmlListProperty<Scene>* property) { reinterpret_cast<World*>(property->data)->clearScenes(); },
        [](QQmlListProperty<Scene>* property, qsizetype index, Scene* p) { reinterpret_cast<World*>(property->data)->replaceScene(index, p); },
        [](QQmlListProperty<Scene>* property) { reinterpret_cast<World*>(property->data)->removeLastScene(); }
    };
}

void World::updateView() const {
    for (auto& scene: sceneList_) {
        scene->draw();
    }
}

void World::updateState(Duration elapsed) {
    for (auto& scene: sceneList_) {
        scene->tick(elapsed);
    }
}

VOLCANO_GAME_END
