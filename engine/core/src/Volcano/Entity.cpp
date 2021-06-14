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

QQmlListProperty<Component> Entity::components(void)
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
    entity->m_components.clear();
}

void Entity::replaceComponent(QQmlListProperty<Component> *list, qsizetype i, Component *component)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    entity->m_components.replace(i, component);
}

void Entity::removeLastComponent(QQmlListProperty<Component> *list)
{
    auto entity = reinterpret_cast<Entity *>(list->data);
    entity->m_components.removeLast();
}

VOLCANO_END
