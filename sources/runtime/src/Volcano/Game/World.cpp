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

void World::update(float elapsed)
{
    for (auto object: m_objects)
        object->update(elapsed);
}

const QList<Object *> &World::objects(void) const
{
    return m_objects;
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

void World::appendObject(Object *object)
{
    m_objects.append(object);
    handleObjectAdded(object);
}

qsizetype World::objectCount(void) const
{
    return m_objects.count();
}

Object *World::objectAt(qsizetype index)
{
    if (0 <= index && index < m_objects.size())
        return m_objects.at(index);

    return nullptr;
}

void World::clearObjects(void)
{
    QList<Object *> objects = std::move(m_objects);
    for (auto object: objects)
        handleObjectRemoved(object);
}

void World::replaceObject(qsizetype index, Object *object)
{
    if (0 <= index && index < m_objects.size()) {
        auto oldObject = m_objects.at(index);
        m_objects.replace(index, object);
        handleObjectRemoved(oldObject);
        handleObjectAdded(object);
    }
}

void World::removeLastObject(void)
{
    if (!m_objects.isEmpty()) {
        auto object = m_objects.last();
        m_objects.removeLast();
        handleObjectRemoved(object);
    }
}

void World::handleObjectAdded(Object *object, bool emitSignal)
{
    object->setParent(this);
    if (emitSignal)
        emit objectAdded(object);
}

void World::handleObjectRemoved(Object *object, bool emitSignal)
{
    object->setParent(nullptr);
    if (emitSignal)
        emit objectRemoved(object);
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
