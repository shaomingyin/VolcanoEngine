//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    WorldBase(parent)
{
}

World::~World(void)
{
}

QQmlListProperty<Object> World::qmlObjects(void)
{
    return { this, this,
        &World::qmlAppendObject,
        &World::qmlObjectCount,
        &World::qmlObjectAt,
        &World::qmlClearObjects,
        &World::qmlReplaceObject,
        &World::qmlRemoveLastObject };
}

void World::handleObjectAdded(Object *object, bool emitSignal)
{
    object->setParent(this);
    WorldBase::handleObjectAdded(object, emitSignal);
}

void World::handleObjectRemoved(Object *object, bool emitSignal)
{
    object->setParent(nullptr);
    WorldBase::handleObjectRemoved(object, emitSignal);
}

void World::qmlAppendObject(QQmlListProperty<Object> *list, Object *object)
{
    reinterpret_cast<World *>(list->data)->appendObject(object);
}

qsizetype World::qmlObjectCount(QQmlListProperty<Object> *list)
{
    return reinterpret_cast<World *>(list->data)->objectCount();
}

Object *World::qmlObjectAt(QQmlListProperty<Object> *list, qsizetype i)
{
    return reinterpret_cast<World *>(list->data)->objectAt(i);
}

void World::qmlClearObjects(QQmlListProperty<Object> *list)
{
    reinterpret_cast<World *>(list->data)->clearObjects();
}

void World::qmlReplaceObject(QQmlListProperty<Object> *list, qsizetype i, Object *object)
{
    reinterpret_cast<World *>(list->data)->replaceObject(i, object);
}

void World::qmlRemoveLastObject(QQmlListProperty<Object> *list)
{
    reinterpret_cast<World *>(list->data)->removeLastObject();
}

VOLCANO_GAME_END
