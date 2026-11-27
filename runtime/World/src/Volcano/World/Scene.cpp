//
//
#include <QtQml/qqml.h>

#include <Volcano/World/BoxCollisionShape.h>
#include <Volcano/World/Camera.h>
#include <Volcano/World/CapsuleCollisionShape.h>
#include <Volcano/World/ColorEnvironment.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/RigidBody.h>
#include <Volcano/World/Scene.h>
#include <Volcano/World/SkyboxEnvironment.h>
#include <Volcano/World/SphereCollisionShape.h>
#include <Volcano/World/SpotLight.h>

VOLCANO_WORLD_BEGIN

Scene::Scene(QObject* parent)
    : QObject(parent) {
}

Scene::~Scene() {
    emit aboutToBeDestroyed();
    clearEntities();
}

void Scene::registerQmlTypes() {
    qmlRegisterType<Scene>("Volcano.World", 1, 0, "Scene");
    qmlRegisterType<Entity>("Volcano.World", 1, 0, "Entity");
    qmlRegisterType<Model>("Volcano.World", 1, 0, "Model");
    qmlRegisterType<Camera>("Volcano.World", 1, 0, "Camera");
    qmlRegisterUncreatableType<Environment>("Volcano.World", 1, 0, "Environment",
                                            QStringLiteral("Use a concrete environment type."));
    qmlRegisterType<ColorEnvironment>("Volcano.World", 1, 0, "ColorEnvironment");
    qmlRegisterType<SkyboxEnvironment>("Volcano.World", 1, 0, "SkyboxEnvironment");
    qmlRegisterUncreatableType<Light>("Volcano.World", 1, 0, "Light",
                                     QStringLiteral("Use a concrete light type."));
    qmlRegisterType<DirectionalLight>("Volcano.World", 1, 0, "DirectionalLight");
    qmlRegisterType<PointLight>("Volcano.World", 1, 0, "PointLight");
    qmlRegisterType<SpotLight>("Volcano.World", 1, 0, "SpotLight");
    qmlRegisterType<RigidBody>("Volcano.World", 1, 0, "RigidBody");
    qmlRegisterUncreatableType<CollisionShape>("Volcano.World", 1, 0, "CollisionShape",
                                               QStringLiteral("Use a concrete collision shape."));
    qmlRegisterType<BoxCollisionShape>("Volcano.World", 1, 0, "BoxCollisionShape");
    qmlRegisterType<CapsuleCollisionShape>("Volcano.World", 1, 0, "CapsuleCollisionShape");
    qmlRegisterType<SphereCollisionShape>("Volcano.World", 1, 0, "SphereCollisionShape");
}

const QVector3D& Scene::gravity() const noexcept {
    return gravity_;
}

void Scene::setGravity(const QVector3D& gravity) {
    if (gravity_ != gravity) {
        gravity_ = gravity;
        emit gravityChanged(gravity_);
    }
}

Environment* Scene::environment() const noexcept {
    return environment_;
}

void Scene::setEnvironment(Environment* environment) {
    if (environment_ == environment) {
        return;
    }
    if (environment_) {
        disconnect(environment_, &QObject::destroyed, this, nullptr);
    }
    environment_ = environment;
    if (environment_) {
        connect(environment_, &QObject::destroyed, this, [this] { emit environmentChanged(nullptr); });
    }
    emit environmentChanged(environment_);
}

const EntityList& Scene::entities() const noexcept {
    return entities_;
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
    connect(entity, &QObject::destroyed, this, [this, entity] {
        entities_.removeAll(entity);
    });
    emit entityAdded(entity);
}

void Scene::handleComponentAdded(Entity* entity, QObject* component) {
    emit componentAdded(entity, component);
}

void Scene::handleEntityRemoved(Entity* entity) {
    disconnect(entity, &Entity::componentAdded, this, nullptr);
    disconnect(entity, &Entity::componentRemoved, this, nullptr);
    disconnect(entity, &QObject::destroyed, this, nullptr);
    auto& components = entity->components();
    for (auto& component: components) {
        emit componentRemoved(entity, component);
    }
    emit entityRemoved(entity);
}

void Scene::handleComponentRemoved(Entity* entity, QObject* component) {
    emit componentRemoved(entity, component);
}

VOLCANO_WORLD_END
