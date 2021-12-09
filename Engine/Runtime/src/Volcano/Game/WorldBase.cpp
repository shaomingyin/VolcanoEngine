//
//
#include <Volcano/Game/WorldBase.hpp>

VOLCANO_GAME_BEGIN

WorldBase::WorldBase(QObject *parent):
    QObject(parent)
{
}

WorldBase::~WorldBase(void)
{
}

void WorldBase::tick(Duration elapsed)
{
    for (auto object: m_objects)
        object->tick(elapsed);
}

const QList<Object *> &WorldBase::objects(void) const
{
    return m_objects;
}

void WorldBase::appendObject(Object *object)
{
    m_objects.append(object);
    handleObjectAdded(object);
}

qsizetype WorldBase::objectCount(void) const
{
    return m_objects.count();
}

Object *WorldBase::objectAt(qsizetype index)
{
    if (0 <= index && index < m_objects.size())
        return m_objects.at(index);
    return nullptr;
}

const Object *WorldBase::objectAt(qsizetype index) const
{
    if (0 <= index && index < m_objects.size())
        return m_objects.at(index);
    return nullptr;
}

void WorldBase::clearObjects(void)
{
    QList<Object *> objects = std::move(m_objects);
    for (auto object: objects)
        handleObjectRemoved(object);
}

void WorldBase::replaceObject(qsizetype index, Object *object)
{
    if (0 <= index && index < m_objects.size()) {
        auto oldObject = m_objects.at(index);
        m_objects.replace(index, object);
        handleObjectRemoved(oldObject);
        handleObjectAdded(object);
    }
}

void WorldBase::removeLastObject(void)
{
    if (!m_objects.isEmpty()) {
        auto object = m_objects.last();
        m_objects.removeLast();
        handleObjectRemoved(object);
    }
}

void WorldBase::handleObjectAdded(Object *object, bool emitSignal)
{
    if (emitSignal)
        emit objectAdded(object);
}

void WorldBase::handleObjectRemoved(Object *object, bool emitSignal)
{
    if (emitSignal)
        emit objectRemoved(object);
}

VOLCANO_GAME_END
