//
//
#include <Volcano/World/Listener.h>

VOLCANO_WORLD_BEGIN

Listener::Listener(Scene& scene, QObject* parent)
    : QObject(parent)
    , scene_(scene) {
    connect(&scene, &Scene::entityAdded, this, &Listener::onEntityAdded);
    connect(&scene, &Scene::entityRemoved, this, &Listener::onEntityRemoved);
}

void Listener::onEntityAdded(Entity* entity) {
    entity_signal_connections_.insert(entity, connect(entity, &Entity::componentAdded, std::bind(&Listener::onComponentAdded, this, entity, std::placeholders::_1)));
    entity_signal_connections_.insert(entity, connect(entity, &Entity::componentRemoved, std::bind(&Listener::onComponentAdded, this, entity, std::placeholders::_1)));
}

void Listener::onEntityRemoved(Entity* entity) {
    auto [i, end] = entity_signal_connections_.equal_range(entity);
    while (i != end) {
        disconnect(i.value());
        ++i;
    }
    entity_signal_connections_.remove(entity);
}

void Listener::onComponentAdded(Entity* entity, Component* component) {
}

void Listener::onComponentRemoved(Entity* entity, Component* component) {
}

VOLCANO_WORLD_END
