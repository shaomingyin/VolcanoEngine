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

void Entity::init(entt::registry& registry) {
    handle_ = entt::handle(registry, registry.create());
}

entt::handle Entity::handle() noexcept {
    return handle_;
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

const ComponentList& Entity::components() const noexcept {
    return components_;
}

void Entity::appendComponent(Component* p) {
    components_.append(p);
    p->attach(handle_);
    emit componentAdded(p);
}

Component* Entity::componentAt(qsizetype i) {
    return components_.at(i);
}

void Entity::clearComponents() {
    for (Component* p: components_) {
        emit componentRemoved(p);
        p->detach();
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
        last->detach();
        emit componentRemoved(last);
    }
}

void Entity::replaceComponent(qsizetype i, Component* p) {
    if (0 <= i && i < components_.count()) {
        auto old = components_.at(i);
        old->detach();
        emit componentRemoved(old);
        components_.replace(i, p);
        p->attach(handle_);
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
