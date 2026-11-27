//
//
#include <utility>

#include <Volcano/World/Entity.h>

VOLCANO_WORLD_BEGIN

Entity::Entity(QObject* parent)
    : QObject(parent) {
}

Entity::~Entity() {
    clearComponents();
}

Transform* Entity::transform() noexcept {
    return &transform_;
}

bool Entity::isEnabled() const noexcept {
    return enabled_;
}

void Entity::setEnabled(bool v) noexcept {
    if (enabled_ != v) {
        enabled_ = v;
        emit enabledChanged(v);
    }
}

const QObjectList& Entity::components() const noexcept {
    return components_;
}

void Entity::appendComponent(QObject* p) {
    components_.append(p);
    if (p) {
        connect(p, &QObject::destroyed, this, &Entity::handleComponentDestroyed, Qt::UniqueConnection);
    }
    emit componentAdded(p);
}

void Entity::handleComponentDestroyed(QObject* component) {
    if (components_.removeAll(component) > 0) {
        emit componentRemoved(component);
    }
}

QObject* Entity::componentAt(qsizetype i) {
    return components_.at(i);
}

void Entity::clearComponents() {
    const auto components = std::exchange(components_, {});
    for (QObject* p: components) {
        if (p) {
            disconnect(p, &QObject::destroyed, this, &Entity::handleComponentDestroyed);
        }
        emit componentRemoved(p);
    }
}

qsizetype Entity::componentCount() {
    return components_.count();
}

void Entity::removeLastComponent() {
    if (!components_.isEmpty()) {
        auto last = components_.last();
        components_.removeLast();
        if (last && !components_.contains(last)) {
            disconnect(last, &QObject::destroyed, this, &Entity::handleComponentDestroyed);
        }
        emit componentRemoved(last);
    }
}

void Entity::replaceComponent(qsizetype i, QObject* p) {
    if (0 <= i && i < components_.count()) {
        auto old = components_.at(i);
        components_.replace(i, p);
        if (old && !components_.contains(old)) {
            disconnect(old, &QObject::destroyed, this, &Entity::handleComponentDestroyed);
        }
        if (p) {
            connect(p, &QObject::destroyed, this, &Entity::handleComponentDestroyed, Qt::UniqueConnection);
        }
        emit componentRemoved(old);
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
