//
//
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

Entity::Entity(QObject *parent)
    : Actor(parent)
{
}

Entity::~Entity(void)
{
}

void Entity::appendComponent(Component *p)
{
    m_componentList.append(p);
}

Component *Entity::componentAt(qsizetype index)
{
    return m_componentList.at(index);
}

const Component *Entity::componentAt(qsizetype index) const
{
    return m_componentList.at(index);
}

void Entity::clearComponents(void)
{
    m_componentList.clear();
}

qsizetype Entity::componentCount(void) const
{
    return m_componentList.count();
}

void Entity::removeLastComponent(void)
{
    m_componentList.removeLast();
}

void Entity::replaceComponent(qsizetype index, Component *p)
{
    m_componentList.replace(index, p);
}

const QList<Component *> &Entity::components(void) const
{
    return m_componentList;
}

VOLCANO_GAME_END
