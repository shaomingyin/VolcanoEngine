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

void Entity::qmlAppendComponent(QQmlListProperty<Component> *list, Component *component)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    entity->m_components.append(component);
    component->setParent(entity);
    emit entity->componentAdded(component);
}

qsizetype Entity::qmlComponentCount(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    return entity->m_components.count();
}

Component *Entity::qmlComponentAt(QQmlListProperty<Component> *list, qsizetype i)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    return entity->m_components.at(i);
}

void Entity::qmlClearComponents(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    auto backup = std::move(entity->m_components);
    for (auto c: backup)
        emit entity->componentRemoved(c);
}

void Entity::qmlReplaceComponent(QQmlListProperty<Component> *list, qsizetype i, Component *component)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    auto oldComponent = entity->m_components.at(i);
    entity->m_components.replace(i, component);
    emit entity->componentRemoved(oldComponent);
    emit entity->componentAdded(component);
    component->setParent(entity);
}

void Entity::qmlRemoveLastComponent(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    if (!entity->m_components.isEmpty()) {
        auto backup = entity->m_components.last();
        entity->m_components.removeLast();
        emit entity->componentRemoved(backup);
    }
}

VOLCANO_GAME_END
