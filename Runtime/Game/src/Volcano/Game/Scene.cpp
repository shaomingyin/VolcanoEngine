//
//
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

Scene::Scene(QObject *parent)
    : Object(parent) {
}

Scene::Scene(Context& context, QObject *parent)
    : Object(context, parent) {
}

const ActorList &Scene::actorList(void) const {
    return actorList_;
}

const EntityList& Scene::entityList() const {
    return entityList_;
}

Light* Scene::ambientLight() {
    return ambientLight_;
}

void Scene::setAmbientLight(Light* p) {
    if (ambientLight_ != p) {
        ambientLight_ = p;
        emit ambientLightChanged(p);
    }
}

const DirectionalLightList& Scene::directionalLightList() const {
    return directionalLightList_;
}

const SpotLightList& Scene::spotLightList() const {
    return spotLightList_;
}

const PointLightList& Scene::pointLightList() const {
    return pointLightList_;
}

void Scene::appendActor(Actor* actor) {
    actorList_.append(actor);
    handleActorAdded(actor);
}

qsizetype Scene::actorCount(void) const {
    return actorList_.count();
}

Actor *Scene::actorAt(qsizetype index) {
    return actorList_.at(index);
}

void Scene::clearActors(void) {
    for (Actor* actor: actorList_) {
        handleActorRemoved(actor);
    }
    actorList_.clear();
    entityList_.clear();
    directionalLightList_.clear();
    spotLightList_.clear();
    pointLightList_.clear();
}

void Scene::replaceActor(qsizetype index, Actor* actor) {
    if (index < actorList_.count()) {
        handleActorRemoved(actorList_.at(index));
        actorList_.replace(index, actor);
        handleActorAdded(actor);
    }
}

void Scene::removeLastActor(void) {
    if (!actorList_.isEmpty()) {
        Actor* last = actorList_.last();
        actorList_.removeLast();
        handleActorRemoved(last);
    }
}

QQmlListProperty<Actor> Scene::qmlActorList() {
    return { this, this,
        [](QQmlListProperty<Actor>* property, Actor* p) { reinterpret_cast<Scene*>(property->data)->appendActor(p); },
        [](QQmlListProperty<Actor>* property) { return reinterpret_cast<Scene*>(property->data)->actorCount(); },
        [](QQmlListProperty<Actor>* property, qsizetype index) { return reinterpret_cast<Scene*>(property->data)->actorAt(index); },
        [](QQmlListProperty<Actor>* property) { reinterpret_cast<Scene*>(property->data)->clearActors(); },
        [](QQmlListProperty<Actor>* property, qsizetype index, Actor* p) { reinterpret_cast<Scene*>(property->data)->replaceActor(index, p); },
        [](QQmlListProperty<Actor>* property) { reinterpret_cast<Scene*>(property->data)->removeLastActor(); }
    };
}

void Scene::updateView() const {
}

void Scene::updateState(Duration elapsed) {
}

void Scene::handleActorAdded(Actor* actor) {
    emit actorAdded(actor);
    Entity* entity = qobject_cast<Entity*>(actor);
    if (entity != nullptr) {
        entityList_.append(entity);
        emit entityAdded(entity);
        return;
    }
    SpotLight* spotLight = qobject_cast<SpotLight*>(actor);
    if (spotLight != nullptr) {
        spotLightList_.append(spotLight);
        emit spotLightAdded(spotLight);
        return;
    }
    DirectionalLight* directionalLight = qobject_cast<DirectionalLight*>(actor);
    if (directionalLight != nullptr) {
        directionalLightList_.append(directionalLight);
        emit directionalLightAdded(directionalLight);
        return;
    }
    PointLight* pointLight = qobject_cast<PointLight*>(actor);
    if (pointLight != nullptr) {
        pointLightList_.append(pointLight);
        emit pointLightAdded(pointLight);
        return;
    }
}

void Scene::handleActorRemoved(Actor* actor) {
    emit actorRemoved(actor);
    Entity* entity = qobject_cast<Entity*>(actor);
    if (entity != nullptr) {
        entityList_.removeAll(entity);
        emit entityRemoved(entity);
        return;
    }
    SpotLight* spotLight = qobject_cast<SpotLight*>(actor);
    if (spotLight != nullptr) {
        spotLightList_.removeAll(spotLight);
        emit spotLightRemoved(spotLight);
        return;
    }
    DirectionalLight* directionalLight = qobject_cast<DirectionalLight*>(actor);
    if (directionalLight != nullptr) {
        directionalLightList_.removeAll(directionalLight);
        emit directionalLightRemoved(directionalLight);
        return;
    }
    PointLight* pointLight = qobject_cast<PointLight*>(actor);
    if (pointLight != nullptr) {
        pointLightList_.removeAll(pointLight);
        emit pointLightRemoved(pointLight);
        return;
    }
}

VOLCANO_GAME_END
