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

void World::update(float elapsed)
{
}

const QList<Object *> &World::objects(void) const
{
    return m_objects;
}

QQmlListProperty<Object> World::qmlObjects(void)
{
    return { this, this,
        &World::appendObject,
        &World::objectCount,
        &World::objectAt,
        &World::clearObjects,
        &World::replaceObject,
        &World::removeLastObject };
}

void World::appendObject(QQmlListProperty<Object> *list, Object *Object)
{
    auto world = reinterpret_cast<World *>(list->data);
    world->m_objects.append(Object);
    emit world->objectAdded(Object);
}

qsizetype World::objectCount(QQmlListProperty<Object> *list)
{
    auto world = reinterpret_cast<World *>(list->data);
    return world->m_objects.count();
}

Object *World::objectAt(QQmlListProperty<Object> *list, qsizetype i)
{
    auto world = reinterpret_cast<World *>(list->data);
    return world->m_objects.at(i);
}

void World::clearObjects(QQmlListProperty<Object> *list)
{
    auto world = reinterpret_cast<World *>(list->data);
    QList<Object *> backup = std::move(world->m_objects);
    for (auto s: backup)
        emit world->objectRemoved(s);
}

void World::replaceObject(QQmlListProperty<Object> *list, qsizetype i, Object *Object)
{
    auto world = reinterpret_cast<World *>(list->data);
    auto oldObject = world->m_objects.at(i);
    world->m_objects.replace(i, Object);
    emit world->objectRemoved(oldObject);
    emit world->objectAdded(Object);
}

void World::removeLastObject(QQmlListProperty<Object> *list)
{
    auto world = reinterpret_cast<World *>(list->data);
    if (!world->m_objects.isEmpty()) {
        auto backup = world->m_objects.last();
        world->m_objects.removeLast();
        emit world->objectRemoved(backup);
    }
}

VOLCANO_END
