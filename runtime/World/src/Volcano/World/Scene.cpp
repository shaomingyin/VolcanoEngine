//
//
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene(QObject* parent)
    : QObject(parent) {
}

Scene::~Scene() {
    clearEntities();
}

void Scene::appendEntity(Entity* p) {
    entities_.append(p);
    handleEntityAdded(p);
}

Entity* Scene::entityAt(qsizetype i) {
    return entities_.at(i);
}

void Scene::clearEntities() {
    for (Entity* p: entities_) {
        handleEntityRemoved(p);
    }
    entities_.clear();
}

qsizetype Scene::entityCount() {
    return entities_.count();
}

void Scene::removeLastEntity() {
    if (!entities_.isEmpty()) {
        handleEntityRemoved(entities_.last());
        entities_.removeLast();
    }
}

void Scene::replaceEntity(qsizetype i, Entity* p) {
    if (0 <= i && i < entities_.count()) {
        handleEntityRemoved(entities_.at(i));
        entities_.replace(i, p);
        handleEntityAdded(p);
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

void Scene::handleEntityAdded(Entity* entity) {
    auto& components = entity->components();
    for (auto& component: components) {
        emit componentAdded(entity, component);
    }
    connect(entity, &Entity::componentAdded, this, [this, entity](QObject* component) {
        handleComponentAdded(entity, component);
    });
    connect(entity, &Entity::componentRemoved, this, [this, entity](QObject* component) {
        handleComponentRemoved(entity, component);
    });
    emit entityAdded(entity);
}

void Scene::handleComponentAdded(Entity* entity, QObject* component) {
    emit componentAdded(entity, component);
}

void Scene::handleEntityRemoved(Entity* entity) {
    auto& components = entity->components();
    for (auto& component: components) {
        emit componentRemoved(entity, component);
    }
    emit entityRemoved(entity);
}

void Scene::handleComponentRemoved(Entity* entity, QObject* component) {
    emit componentRemoved(entity, component);
}

void Scene::update(Clock::duration elapsed) {
}

VOLCANO_WORLD_END
