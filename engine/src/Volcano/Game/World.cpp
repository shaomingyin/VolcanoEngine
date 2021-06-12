//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    QObject(parent)
{
}

World::~World(void)
{
}

QQmlListProperty<Scene> World::scenes(void)
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
    world->m_scenes.clear();
}

void World::replaceScene(QQmlListProperty<Scene> *list, qsizetype i, Scene *scene)
{
    auto world = reinterpret_cast<World *>(list->data);
    world->m_scenes.replace(i, scene);
}

void World::removeLastScene(QQmlListProperty<Scene> *list)
{
    auto world = reinterpret_cast<World *>(list->data);
    world->m_scenes.removeLast();
}

VOLCANO_GAME_END
