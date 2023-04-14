//
//
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

Scene::Scene(QObject *parent)
    : Object(parent)
{
}

Scene::~Scene(void)
{
}

const EntityList &Scene::entityList(void) const
{
    return m_entityList;
}

void Scene::appendEntity(Entity *entity)
{
    m_entityList.append(entity);
    emit entityAdded(entity);
}

Entity *Scene::entityAt(qsizetype index)
{
    return m_entityList.at(index);
}

void Scene::clearEntities(void)
{
    for (Entity *entity: m_entityList)
        emit entityRemoved(entity);
    m_entityList.clear();
}

qsizetype Scene::entityCount(void) const
{
    return m_entityList.count();
}

void Scene::removeLastEntity(void)
{
    if (!m_entityList.isEmpty()) {
        emit entityRemoved(m_entityList.last());
        m_entityList.removeLast();
    }
}

void Scene::replaceEntity(qsizetype index, Entity *entity)
{
    if (index < m_entityList.count()) {
        emit entityRemoved(m_entityList.last());
        m_entityList.replace(index, entity);
        emit entityAdded(entity);
    }
}

VOLCANO_GAME_END
