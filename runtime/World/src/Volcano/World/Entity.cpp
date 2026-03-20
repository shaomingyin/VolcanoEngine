//
//
#include <Volcano/World/Entity.h>

VOLCANO_WORLD_BEGIN

Entity::Entity(QObject* parent)
    : QObject(parent) {
}

Entity::~Entity() {
    clearComponents();
}

void Entity::appendComponent(QObject* p) {
    components_.append(p);
    p->setParent(this);
    emit componentAdded(p);
}

QObject* Entity::componentAt(qsizetype i) {
    return components_.at(i);
}

void Entity::clearComponents() {
    for (QObject* p: components_) {
        emit componentRemoved(p);
    }
    components_.clear();
}

qsizetype Entity::componentCount() {
    return components_.count();
}

void Entity::removeLastComponent() {
    if (!components_.isEmpty()) {
        auto last = components_.last();
        components_.removeLast();
        emit componentRemoved(last);
    }
}

void Entity::replaceComponent(qsizetype i, QObject* p) {
    if (0 <= i && i < components_.count()) {
        auto old = components_.at(i);
        emit componentRemoved(old);
        components_.replace(i, p);
        emit componentAdded(p);
    }
}

QQmlListProperty<QObject> Entity::qmlComponents() {
    return { this, this,
        [](QQmlListProperty<QObject>* prop, QObject* p) { reinterpret_cast<Entity*>(prop->data)->appendComponent(p); },
        [](QQmlListProperty<QObject>* prop) { return reinterpret_cast<Entity*>(prop->data)->componentCount(); },
        [](QQmlListProperty<QObject>* prop, qsizetype i) { return reinterpret_cast<Entity*>(prop->data)->componentAt(i); },
        [](QQmlListProperty<QObject>* prop) { reinterpret_cast<Entity*>(prop->data)->clearComponents(); },
        [](QQmlListProperty<QObject>* prop, qsizetype i, QObject* p) { reinterpret_cast<Entity*>(prop->data)->replaceComponent(i, p); },
        [](QQmlListProperty<QObject>* prop) { reinterpret_cast<Entity*>(prop->data)->removeLastComponent(); }
    };
}

VOLCANO_WORLD_END
