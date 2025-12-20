//
//
#include <Volcano/Game/BoxRigidBody.h>
#include <Volcano/Game/Scene.h>

VOLCANO_GAME_BEGIN

Scene::Scene(QObject* parent)
    : Object(parent) {
}

Scene::~Scene() {
    clearEntities();
}

void Scene::update(Clock::duration elapsed) {
}

void Scene::appendEntity(Entity* p) {
    entities_.append(p);
    onEntityAdded(p);
}

Entity* Scene::entityAt(qsizetype i) {
    return entities_.at(i);
}

void Scene::clearEntities() {
    for (Entity* p: entities_) {
        onEntityRemoved(p);
    }
    entities_.clear();
}

qsizetype Scene::entityCount() {
    return entities_.count();
}

void Scene::removeLastEntity() {
    if (!entities_.isEmpty()) {
        onEntityRemoved(entities_.last());
        entities_.removeLast();
    }
}

void Scene::replaceEntity(qsizetype i, Entity* p) {
    if (0 <= i && i < entities_.count()) {
        onEntityRemoved(entities_.at(i));
        entities_.replace(i, p);
        onEntityAdded(p);
    }
}

QQmlListProperty<Entity> Scene::qmlEntities() {
    return { this, this,
        [](QQmlListProperty<Entity>* prop, Entity* p) { reinterpret_cast<Scene*>(prop->data)->appendEntity(p); },
        [](QQmlListProperty<Entity>* prop) { return reinterpret_cast<Scene*>(prop->data)->entityCount(); },
        [](QQmlListProperty<Entity>* prop, qsizetype i) { return reinterpret_cast<Scene*>(prop->data)->entityAt(i); },
        [](QQmlListProperty<Entity>* prop) { reinterpret_cast<Scene*>(prop->data)->clearEntities(); },
        [](QQmlListProperty<Entity>* prop, qsizetype i, Entity* p) { reinterpret_cast<Scene*>(prop->data)->replaceEntity(i, p); },
        [](QQmlListProperty<Entity>* prop) { reinterpret_cast<Scene*>(prop->data)->removeLastEntity(); }
    };
}

void Scene::onEntityAdded(Entity* entity) {
    auto& components = entity->components();
    for (auto& component: components) {
        emit componentAdded(entity, component);
    }
    connect(entity, &Entity::componentAdded, this, [this, entity](Component* component) {
        onComponentAdded(entity, component);
    });
    connect(entity, &Entity::componentRemoved, this, [this, entity](Component* component) {
        onComponentRemoved(entity, component);
    });
    emit entityAdded(entity);
}

void Scene::onComponentAdded(Entity* entity, Component* component) {
    emit componentAdded(entity, component);
}

void Scene::onEntityRemoved(Entity* entity) {
    auto& components = entity->components();
    for (auto& component: components) {
        emit componentRemoved(entity, component);
    }
    emit entityRemoved(entity);
}

void Scene::onComponentRemoved(Entity* entity, Component* component) {
    emit componentRemoved(entity, component);
}

VOLCANO_GAME_END
