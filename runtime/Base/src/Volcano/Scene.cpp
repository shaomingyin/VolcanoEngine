//
//
#include <Volcano/Scene.h>

VOLCANO_BEGIN

Scene::Scene(QObject* parent)
    : QObject(parent) {
}

void Scene::update(Duration elapsed) {
    dynamic_.update(elapsed);
}

void Scene::appendEntity(Entity* p) {
    entities_.append(p);
    emit entityAdded(p);
}

Entity* Scene::entityAt(qsizetype i) {
    return entities_.at(i);
}

void Scene::clearEntities() {
    for (Entity* p: entities_) {
        emit entityRemoved(p);
    }
    entities_.clear();
}

qsizetype Scene::entityCount() {
    return entities_.count();
}

void Scene::removeLastEntity() {
    if (!entities_.isEmpty()) {
        emit entityRemoved(entities_.last());
        entities_.removeLast();
    }
}

void Scene::replaceEntity(qsizetype i, Entity* p) {
    if (0 <= i && i < entities_.count()) {
        emit entityRemoved(entities_.at(i));
        entities_.replace(i, p);
        emit entityAdded(p);
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

VOLCANO_END
