//
//
#include <Volcano/Game/BoxRigidBody.h>
#include <Volcano/Game/World.h>

VOLCANO_GAME_BEGIN

World::World(QObject* parent)
    : Object(parent) {
}

World::~World() {
    clearScenes();
}

void World::update(Clock::duration elapsed) {
    dynamic_.update(elapsed);
}

void World::appendScene(Scene* p) {
    scenes_.append(p);
    onSceneAdded(p);
}

Scene* World::sceneAt(qsizetype i) {
    return scenes_.at(i);
}

void World::clearScenes() {
    for (Scene* p: scenes_) {
        onSceneRemoved(p);
    }
    scenes_.clear();
}

qsizetype World::sceneCount() {
    return scenes_.count();
}

void World::removeLastScene() {
    if (!scenes_.isEmpty()) {
        onSceneRemoved(scenes_.last());
        scenes_.removeLast();
    }
}

void World::replaceScene(qsizetype i, Scene* p) {
    if (0 <= i && i < scenes_.count()) {
        onSceneRemoved(scenes_.at(i));
        scenes_.replace(i, p);
        onSceneAdded(p);
    }
}

QQmlListProperty<Scene> World::qmlScenes() {
    return { this, this,
        [](QQmlListProperty<Scene>* prop, Scene* p) { reinterpret_cast<World*>(prop->data)->appendScene(p); },
        [](QQmlListProperty<Scene>* prop) { return reinterpret_cast<World*>(prop->data)->sceneCount(); },
        [](QQmlListProperty<Scene>* prop, qsizetype i) { return reinterpret_cast<World*>(prop->data)->sceneAt(i); },
        [](QQmlListProperty<Scene>* prop) { reinterpret_cast<World*>(prop->data)->clearScenes(); },
        [](QQmlListProperty<Scene>* prop, qsizetype i, Scene* p) { reinterpret_cast<World*>(prop->data)->replaceScene(i, p); },
        [](QQmlListProperty<Scene>* prop) { reinterpret_cast<World*>(prop->data)->removeLastScene(); }
    };
}

void World::onSceneAdded(Scene* scene) {
    // auto& components = scene->components();
    // for (auto& component: components) {
    //     emit componentAdded(entity, component);
    // }
    // connect(scene, &Scene::entityAdded, this, [this, scene](Entity* entity) {
    //     onComponentAdded(scene, entity, component);
    // });
    // connect(entity, &Entity::componentRemoved, this, [this, entity](Component* component) {
    //     onComponentRemoved(entity, component);
    // });
    emit sceneAdded(scene);
}

void World::onComponentAdded(Scene* scene, Entity* entity, Component* component) {
    auto rigid_body = qobject_cast<RigidBody*>(component);
    if (rigid_body != nullptr) {
        rigid_body->setParentTransform(entity->transform());
        dynamic_.addRigidBody(rigid_body);
    }
    emit componentAdded(scene, entity, component);
}

void World::onSceneRemoved(Scene* scene) {
    // auto& components = entity->components();
    // for (auto& component: components) {
    //     emit componentRemoved(entity, component);
    // }
    emit sceneRemoved(scene);
}

void World::onComponentRemoved(Scene* scene, Entity* entity, Component* component) {
    auto rigid_body = qobject_cast<RigidBody*>(component);
    if (rigid_body != nullptr) {
        dynamic_.removeRigidBody(rigid_body);
        rigid_body->setParentTransform(nullptr);
    }
    emit componentRemoved(scene, entity, component);
}

VOLCANO_GAME_END
