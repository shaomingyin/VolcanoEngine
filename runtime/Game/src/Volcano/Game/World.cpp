//
//
#include <Volcano/Game/BoxRigidBody.h>
#include <Volcano/Game/World.h>

VOLCANO_GAME_BEGIN

World::World(QObject* parent)
    : Object(parent) {
}

World::~World() {
    clearEntities();
}

void World::update(Duration elapsed) {
    dynamic_.update(elapsed);
}

void World::appendEntity(Entity* p) {
    entities_.append(p);
    onEntityAdded(p);
}

Entity* World::entityAt(qsizetype i) {
    return entities_.at(i);
}

void World::clearEntities() {
    for (Entity* p: entities_) {
        onEntityRemoved(p);
    }
    entities_.clear();
}

qsizetype World::entityCount() {
    return entities_.count();
}

void World::removeLastEntity() {
    if (!entities_.isEmpty()) {
        onEntityRemoved(entities_.last());
        entities_.removeLast();
    }
}

void World::replaceEntity(qsizetype i, Entity* p) {
    if (0 <= i && i < entities_.count()) {
        onEntityRemoved(entities_.at(i));
        entities_.replace(i, p);
        onEntityAdded(p);
    }
}

QQmlListProperty<Entity> World::qmlEntities() {
    return { this, this,
        [](QQmlListProperty<Entity>* prop, Entity* p) { reinterpret_cast<World*>(prop->data)->appendEntity(p); },
        [](QQmlListProperty<Entity>* prop) { return reinterpret_cast<World*>(prop->data)->entityCount(); },
        [](QQmlListProperty<Entity>* prop, qsizetype i) { return reinterpret_cast<World*>(prop->data)->entityAt(i); },
        [](QQmlListProperty<Entity>* prop) { reinterpret_cast<World*>(prop->data)->clearEntities(); },
        [](QQmlListProperty<Entity>* prop, qsizetype i, Entity* p) { reinterpret_cast<World*>(prop->data)->replaceEntity(i, p); },
        [](QQmlListProperty<Entity>* prop) { reinterpret_cast<World*>(prop->data)->removeLastEntity(); }
    };
}

void World::onEntityAdded(Entity* entity) {
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

void World::onComponentAdded(Entity* entity, Component* component) {
    auto rigid_body = qobject_cast<RigidBody*>(component);
    if (rigid_body != nullptr) {
        rigid_body->attachParentTransform(entity->transform());
        dynamic_.addRigidBody(rigid_body);
    }
    emit componentAdded(entity, component);
}

void World::onEntityRemoved(Entity* entity) {
    auto& components = entity->components();
    for (auto& component: components) {
        emit componentRemoved(entity, component);
    }
    emit entityRemoved(entity);
}

void World::onComponentRemoved(Entity* entity, Component* component) {
    auto rigid_body = qobject_cast<RigidBody*>(component);
    if (rigid_body != nullptr) {
        dynamic_.removeRigidBody(rigid_body);
        rigid_body->attachParentTransform(nullptr);
    }
    emit componentRemoved(entity, component);
}

QDataStream& operator<<(QDataStream& s, const World& v) {
    s << static_cast<const Object&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, World& v) {
    s >> static_cast<Object&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
