//
//
#include <Volcano/World/Entity.h>

VOLCANO_WORLD_BEGIN

void Entity::appendComponent(Component* p) {
    components_.append(p);
    emit componentAdded(p);
}

Component* Entity::componentAt(qsizetype i) {
    return components_.at(i);
}

const Component* Entity::componentAt(qsizetype i) const {
    return components_.at(i);
}

void Entity::clearComponents() {
    for (auto p: components_) {
        componentRemoved(p);
    }
    components_.clear();
}

qsizetype Entity::componentCount() const {
    return components_.count();
}

void Entity::removeLastComponent() {
    if (!components_.isEmpty()) {
        componentRemoved(components_.last());
        components_.removeLast();
    }
}

void Entity::replaceComponent(qsizetype i, Component* p) {
    if (0 <= i && i < components_.count()) {
        componentRemoved(components_.at(i));
        components_.replace(i, p);
    }
}

QQmlListProperty<Component> Entity::qmlComponents() {
    return { this, this,
        [](QQmlListProperty<Component>* lp, Component* p) { reinterpret_cast<Entity*>(lp->data)->appendComponent(p); },
        [](QQmlListProperty<Component>* lp) { return reinterpret_cast<Entity*>(lp->data)->componentCount(); },
        [](QQmlListProperty<Component>* lp, qsizetype i) { return reinterpret_cast<Entity*>(lp->data)->componentAt(i); },
        [](QQmlListProperty<Component>* lp) { reinterpret_cast<Entity*>(lp->data)->clearComponents(); },
        [](QQmlListProperty<Component>* lp, qsizetype i, Component* p) { reinterpret_cast<Entity*>(lp->data)->replaceComponent(i, p); },
        [](QQmlListProperty<Component>* lp) { reinterpret_cast<Entity*>(lp->data)->removeLastComponent(); }
    };
}

VOLCANO_WORLD_END
