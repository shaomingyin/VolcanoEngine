//
//
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

Entity::Entity(QObject *parent):
    QObject(parent)
{
}

Entity::~Entity(void)
{
}

QQmlListProperty<Component> Entity::qmlComponents(void)
{
    return QQmlListProperty<Component>(this, this,
        &Entity::qmlAppendComponent,
        &Entity::qmlComponentCount,
        &Entity::qmlComponentAt,
        &Entity::qmlClearComponents,
        &Entity::qmlReplaceComponent,
        &Entity::qmlRemoveLastComponent);
}

void Entity::update(float elapsed)
{
}

void Entity::appendComponent(Component *comp)
{
    // TODO

    m_componentList.append(comp);
}

void Entity::qmlAppendComponent(QQmlListProperty<Component> *list, Component *comp)
{
    reinterpret_cast<Entity *>(list->data)->appendComponent(comp);
}

int Entity::componentCount(void) const
{
    return m_componentList.count();
}

int Entity::qmlComponentCount(QQmlListProperty<Component> *list)
{
    return reinterpret_cast<Entity *>(list->data)->componentCount();
}

Component *Entity::componentAt(int i)
{
    return m_componentList.at(i);
}

Component *Entity::qmlComponentAt(QQmlListProperty<Component> *list, int i)
{
    return reinterpret_cast<Entity *>(list->data)->componentAt(i);
}

void Entity::clearComponents(void)
{
    // TODO

    m_componentList.clear();
}

void Entity::qmlClearComponents(QQmlListProperty<Component> *list)
{
    reinterpret_cast<Entity *>(list->data)->clearComponents();
}

void Entity::replaceComponent(int i, Component *comp)
{
    // TODO

    m_componentList.replace(i, comp);
}

void Entity::qmlReplaceComponent(QQmlListProperty<Component> *list, int i, Component *comp)
{
    reinterpret_cast<Entity *>(list->data)->replaceComponent(i, comp);
}

void Entity::removeLastComponent(void)
{
    // TODO

    m_componentList.removeLast();
}

void Entity::qmlRemoveLastComponent(QQmlListProperty<Component> *list)
{
    reinterpret_cast<Entity *>(list->data)->removeLastComponent();
}

VOLCANO_GAME_END
