//
//
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene(QObject* parent)
    : QObject(parent) {
}

void Scene::appendEntity(Entity* p) {
    entities_.append(p);
    emit entityAdded(p);
}

Entity* Scene::entityAt(qsizetype i) {
    return entities_.at(i);
}

void Scene::clearEntities() {
    for (auto p: entities_) {
        entityRemoved(p);
    }
    entities_.clear();
}

qsizetype Scene::entityCount() {
    return entities_.count();
}

void Scene::removeLastEntity() {
    if (!entities_.isEmpty()) {
        entityRemoved(entities_.last());
        entities_.removeLast();
    }
}

void Scene::replaceEntity(qsizetype i, Entity* p) {
    if (0 <= i && i < entities_.count()) {
        entityRemoved(entities_.at(i));
        entities_.replace(i, p);
    }
}

QQmlListProperty<Entity> Scene::qmlEntities() {
    return { this, this,
        [](QQmlListProperty<Entity>* lp, Entity* p) { reinterpret_cast<Scene*>(lp->data)->appendEntity(p); },
        [](QQmlListProperty<Entity>* lp) { return reinterpret_cast<Scene*>(lp->data)->entityCount(); },
        [](QQmlListProperty<Entity>* lp, qsizetype i) { return reinterpret_cast<Scene*>(lp->data)->entityAt(i); },
        [](QQmlListProperty<Entity>* lp) { reinterpret_cast<Scene*>(lp->data)->clearEntities(); },
        [](QQmlListProperty<Entity>* lp, qsizetype i, Entity* p) { reinterpret_cast<Scene*>(lp->data)->replaceEntity(i, p); },
        [](QQmlListProperty<Entity>* lp) { reinterpret_cast<Scene*>(lp->data)->removeLastEntity(); }
    };
}

void Scene::update(Duration elapsed) {
}

VOLCANO_WORLD_END
