//
//
#include <Volcano/World.hpp>

VOLCANO_BEGIN

World::World(QObject *parent):
    QObject(parent)
{
}

World::~World(void)
{
}

const QList<Scene *> &World::scenes(void) const
{
    return m_scenes;
}

QQmlListProperty<Scene> World::qmlScenes(void)
{
    return { this, this,
        &World::appendScene,
        &World::sceneCount,
        &World::sceneAt,
        &World::clearScenes,
        &World::replaceScene,
        &World::removeLastScene };
}

void World::appendScene(QQmlListProperty<Scene> *list, Scene *scene)
{
    auto world = reinterpret_cast<World *>(list->data);
    world->m_scenes.append(scene);
    emit world->sceneAdded(scene);
}

qsizetype World::sceneCount(QQmlListProperty<Scene> *list)
{
    auto world = reinterpret_cast<World *>(list->data);
    return world->m_scenes.count();
}

Scene *World::sceneAt(QQmlListProperty<Scene> *list, qsizetype i)
{
    auto world = reinterpret_cast<World *>(list->data);
    return world->m_scenes.at(i);
}

void World::clearScenes(QQmlListProperty<Scene> *list)
{
    auto world = reinterpret_cast<World *>(list->data);
    QList<Scene *> backup = std::move(world->m_scenes);
    for (auto s: backup)
        emit world->sceneRemoved(s);
}

void World::replaceScene(QQmlListProperty<Scene> *list, qsizetype i, Scene *scene)
{
    auto world = reinterpret_cast<World *>(list->data);
    auto oldScene = world->m_scenes.at(i);
    world->m_scenes.replace(i, scene);
    emit world->sceneRemoved(oldScene);
    emit world->sceneAdded(scene);
}

void World::removeLastScene(QQmlListProperty<Scene> *list)
{
    auto world = reinterpret_cast<World *>(list->data);
    if (!world->m_scenes.isEmpty()) {
        auto backup = world->m_scenes.last();
        world->m_scenes.removeLast();
        emit world->sceneRemoved(backup);
    }
}

VOLCANO_END
