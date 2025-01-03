//
//
#include <Volcano/World/Transformable.h>
#include <Volcano/World/Entity.h>

VOLCANO_WORLD_BEGIN

Entity::Entity(QObject* parent)
    : Object(parent) {
}

Entity::~Entity() {
    clearComponents();
}

void Entity::appendComponent(Component* p) {
    components_.append(p);
    auto transformable = qobject_cast<Transformable*>(p);
    if (transformable != nullptr) {
        transformable->attachParentTransform(&transform_.affine());
    }
    emit componentAdded(p);
}

Component* Entity::componentAt(qsizetype i) {
    return components_.at(i);
}

void Entity::clearComponents() {
    for (Component* p: components_) {
        auto transformable = qobject_cast<Transformable*>(p);
        if (transformable != nullptr) {
            transformable->attachParentTransform(nullptr);
        }
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
        auto transformable = qobject_cast<Transformable*>(last);
        if (transformable != nullptr) {
            transformable->attachParentTransform(nullptr);
        }
        components_.removeLast();
        emit componentRemoved(last);
    }
}

void Entity::replaceComponent(qsizetype i, Component* p) {
    if (0 <= i && i < components_.count()) {
        auto old = components_.at(i);
        auto transformable = qobject_cast<Transformable*>(old);
        if (transformable != nullptr) {
            transformable->attachParentTransform(nullptr);
        }
        emit componentRemoved(old);
        components_.replace(i, p);
        transformable = qobject_cast<Transformable*>(p);
        if (transformable != nullptr) {
            transformable->attachParentTransform(&transform_.affine());
        }
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

VOLCANO_WORLD_END
