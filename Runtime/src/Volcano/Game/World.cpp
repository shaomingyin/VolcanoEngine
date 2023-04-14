//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent)
    : Object(parent)
{
}

World::~World(void)
{
}

const SceneList &World::sceneList(void) const
{
    return m_sceneList;
}

void World::appendScene(Scene *scene)
{
    m_sceneList.append(scene);
    emit sceneAdded(scene);
}

Scene *World::sceneAt(qsizetype index)
{
    return m_sceneList.at(index);
}

void World::clearScenes(void)
{
    for (Scene *scene: m_sceneList)
        emit sceneRemoved(scene);
    m_sceneList.clear();
}

qsizetype World::sceneCount(void) const
{
    return m_sceneList.count();
}

void World::removeLastScene(void)
{
    if (!m_sceneList.isEmpty()) {
        emit sceneRemoved(m_sceneList.last());
        m_sceneList.removeLast();
    }
}

void World::replaceScene(qsizetype index, Scene *scene)
{
    if (index < m_sceneList.count()) {
        emit sceneRemoved(m_sceneList.at(index));
        m_sceneList.replace(index, scene);
        emit sceneAdded(scene);
    }
}

VOLCANO_GAME_END
