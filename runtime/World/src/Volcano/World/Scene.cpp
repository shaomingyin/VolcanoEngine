//
//
#include <Volcano/World/BoxRigidBody.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene(QObject* parent)
    : Object(parent) {
}

Scene::~Scene() {
    clearEntities();
}

void Scene::update(Duration elapsed) {
    dynamic_.update(elapsed);
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
    auto rigid_body = qobject_cast<RigidBody*>(component);
    if (rigid_body != nullptr) {
        rigid_body->attachParentTransform(entity->transform());
        dynamic_.addRigidBody(rigid_body);
    }
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
    auto rigid_body = qobject_cast<RigidBody*>(component);
    if (rigid_body != nullptr) {
        dynamic_.removeRigidBody(rigid_body);
        rigid_body->attachParentTransform(nullptr);
    }
    emit componentRemoved(entity, component);
}

VOLCANO_WORLD_END
