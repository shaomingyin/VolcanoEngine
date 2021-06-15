//
//
#include <Volcano/Entity.hpp>

VOLCANO_BEGIN

Entity::Entity(QObject *parent):
    Object(parent)
{
}

Entity::~Entity(void)
{
}

const QList<Component *> &Entity::components(void) const
{
    return m_components;
}

QQmlListProperty<Component> Entity::qmlComponents(void)
{
    return { this, this,
        &Entity::appendComponent,
        &Entity::componentCount,
        &Entity::componentAt,
        &Entity::clearComponents,
        &Entity::replaceComponent,
        &Entity::removeLastComponent };
}

void Entity::appendComponent(QQmlListProperty<Component> *list, Component *component)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    entity->m_components.append(component);
    emit entity->componentAdded(component);
}

qsizetype Entity::componentCount(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    return entity->m_components.count();
}

Component *Entity::componentAt(QQmlListProperty<Component> *list, qsizetype i)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    return entity->m_components.at(i);
}

void Entity::clearComponents(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    auto backup = std::move(entity->m_components);
    for (auto c: backup)
        emit entity->componentRemoved(c);
}

void Entity::replaceComponent(QQmlListProperty<Component> *list, qsizetype i, Component *component)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    auto oldComponent = entity->m_components.at(i);
    entity->m_components.replace(i, component);
    emit entity->componentRemoved(oldComponent);
    emit entity->componentAdded(component);
}

void Entity::removeLastComponent(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    if (!entity->m_components.isEmpty()) {
        auto backup = entity->m_components.last();
        entity->m_components.removeLast();
        emit entity->componentRemoved(backup);
    }
}

VOLCANO_END
