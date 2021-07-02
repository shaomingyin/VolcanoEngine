//
//
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

Entity::Entity(QObject *parent):
    Object(parent)
{
}

Entity::~Entity(void)
{
}

void Entity::update(float elapsed)
{
    for (auto component: m_components)
        component->update(elapsed);
}

const QVector3D &Entity::position(void) const
{
    return m_position;
}

void Entity::setPosition(const QVector3D &v)
{
    if (m_position != v) {
        m_position = v;
        emit positionChanged(v);
    }
}

const QVector3D &Entity::scale(void) const
{
    return m_scale;
}

void Entity::setScale(const QVector3D &v)
{
    if (m_scale != v) {
        m_scale = v;
        emit scaleChanged(v);
    }
}

const QQuaternion &Entity::rotation(void)
{
    return m_rotation;
}

void Entity::setRotation(const QQuaternion &v)
{
    if (m_rotation != v) {
        m_rotation = v;
        emit rotationChanged(v);
    }
}

const QList<Component *> &Entity::components(void) const
{
    return m_components;
}

QQmlListProperty<Component> Entity::qmlComponents(void)
{
    return { this, this,
        &Entity::qmlAppendComponent,
        &Entity::qmlComponentCount,
        &Entity::qmlComponentAt,
        &Entity::qmlClearComponents,
        &Entity::qmlReplaceComponent,
        &Entity::qmlRemoveLastComponent };
}

void Entity::handleComponentAdded(Component *component)
{
    component->setParent(this);
    emit componentAdded(component);
}

void Entity::handleComponentRemoved(Component *component)
{
    component->setParent(nullptr);
    emit componentRemoved(component);
}

void Entity::qmlAppendComponent(QQmlListProperty<Component> *list, Component *component)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    entity->m_components.append(component);
    entity->handleComponentAdded(component);
}

qsizetype Entity::qmlComponentCount(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    return entity->m_components.count();
}

Component *Entity::qmlComponentAt(QQmlListProperty<Component> *list, qsizetype i)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    if (0 <= i && i < entity->m_components.size())
        return entity->m_components.at(i);
    return nullptr;
}

void Entity::qmlClearComponents(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    auto backup = std::move(entity->m_components);
    for (auto component: backup)
        entity->handleComponentRemoved(component);
}

void Entity::qmlReplaceComponent(QQmlListProperty<Component> *list, qsizetype i, Component *component)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    if (0 <= i && i < entity->m_components.size()) {
        auto oldComponent = entity->m_components.at(i);
        entity->m_components.replace(i, component);
        entity->handleComponentRemoved(oldComponent);
        entity->handleComponentAdded(component);
    }
}

void Entity::qmlRemoveLastComponent(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    if (!entity->m_components.isEmpty()) {
        auto backup = entity->m_components.last();
        entity->m_components.removeLast();
        entity->handleComponentRemoved(backup);
    }
}

VOLCANO_GAME_END
