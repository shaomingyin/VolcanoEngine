//
//
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

Entity::Entity(QObject *parent)
    : Actor(parent) {
}

Entity::~Entity(void) {
}

const QList<Component *> &Entity::componentList(void) const {
    return componentList_;
}

void Entity::appendComponent(Component *p) {
    componentList_.append(p);
    emit componentAdded(p);
}

qsizetype Entity::componentCount(void) const {
    return componentList_.count();
}

Component *Entity::componentAt(qsizetype index) {
    return componentList_.at(index);
}

const Component *Entity::componentAt(qsizetype index) const {
    return componentList_.at(index);
}

void Entity::clearComponents(void) {
    for (Component* component: componentList_) {
        emit componentRemoved(component);
    }
    componentList_.clear();
}

void Entity::replaceComponent(qsizetype index, Component *p) {
    if (index < componentList_.count()) {
        emit componentRemoved(componentList_.at(index));
        componentList_.replace(index, p);
        emit componentAdded(p);
    }
}

void Entity::removeLastComponent(void) {
    if (!componentList_.isEmpty()) {
        Component* last = componentList_.last();
        componentList_.removeLast();
        emit componentRemoved(last);
    }
}

VOLCANO_GAME_END
