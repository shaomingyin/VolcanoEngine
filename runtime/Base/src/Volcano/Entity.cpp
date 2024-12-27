//
//
#include <Volcano/Entity.h>

VOLCANO_BEGIN

Entity::Entity(QObject* parent)
    : QObject(parent) {
}

void Entity::appendComponent(Component* p) {
    components_.append(p);
    emit componentAdded(p);
}

Component* Entity::componentAt(qsizetype i) {
    return components_.at(i);
}

void Entity::clearComponents() {
    for (Component* p: components_) {
        emit componentRemoved(p);
    }
    components_.clear();
}

qsizetype Entity::componentCount() {
    return components_.count();
}

void Entity::removeLastComponent() {
    if (!components_.isEmpty()) {
        emit componentRemoved(components_.last());
        components_.removeLast();
    }
}

void Entity::replaceComponent(qsizetype i, Component* p) {
    if (0 <= i && i < components_.count()) {
        emit componentRemoved(components_.at(i));
        components_.replace(i, p);
        emit componentAdded(p);
    }
}

QQmlListProperty<Component> Entity::qmlComponents() {
    return { this, this,
        [](QQmlListProperty<Component>* prop, Component* p) { reinterpret_cast<Entity*>(prop->data)->appendComponent(p); },
        [](QQmlListProperty<Component>* prop) { return reinterpret_cast<Entity*>(prop->data)->componentCount(); },
        [](QQmlListProperty<Component>* prop, qsizetype i) { return reinterpret_cast<Entity*>(prop->data)->componentAt(i); },
        [](QQmlListProperty<Component>* prop) { reinterpret_cast<Entity*>(prop->data)->clearComponents(); },
        [](QQmlListProperty<Component>* prop, qsizetype i, Component* p) { reinterpret_cast<Entity*>(prop->data)->replaceComponent(i, p); },
        [](QQmlListProperty<Component>* prop) { reinterpret_cast<Entity*>(prop->data)->removeLastComponent(); }
    };
}

VOLCANO_END
