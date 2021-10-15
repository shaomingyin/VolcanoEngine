//
//
#include <memory>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    QObject(parent),
    m_btCollisionConfiguration(nullptr),
    m_btDispatcher(nullptr),
    m_btOverlappingPairCache(nullptr),
    m_btSolver(nullptr),
    m_btWorld(nullptr),
    m_gravity(0.0f, -9.8f, 0.0f)
{
}

World::~World(void)
{
    if (m_btWorld != nullptr)
        releaseDynamicWorld();
}

void World::update(float elapsed)
{
    for (auto object: m_objects)
        object->update(elapsed);

    if (m_btWorld != nullptr) {
        m_btWorld->stepSimulation(elapsed);
        // TODO
    }
}

Light *World::ambientLight(void)
{
    return &m_ambientLight;
}

bool World::isDynamic(void) const
{
    return (m_btWorld != nullptr);
}

void World::setDynamic(bool v)
{
    if (isDynamic() == v)
        return;

    if (v) {
        if (initDynamicWorld())
            emit dynamicChanged(true);
    } else {
        releaseDynamicWorld();
        emit dynamicChanged(false);
    }
}

const QVector3D &World::gravity(void) const
{
    return m_gravity;
}

void World::setGravity(const QVector3D &v)
{
    if (m_gravity == v)
        return;

    m_gravity = v;

    if (m_btWorld != nullptr)
        m_btWorld->setGravity(btVector3(m_gravity[0], m_gravity[1], m_gravity[2]));

    emit gravityChanged(v);
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

    if (m_btWorld != nullptr) {
        auto entity = qobject_cast<Entity *>(object);
        if (entity != nullptr) {
            auto components = entity->components();
            for (auto component: components) {
                // TODO
            }
        }
    }

    if (emitSignal)
        emit objectAdded(object);
}

void World::handleObjectRemoved(Object *object, bool emitSignal)
{
    if (m_btWorld != nullptr) {
        // TODO
    }

    object->setParent(nullptr);

    if (emitSignal)
        emit objectRemoved(object);
}

bool World::initDynamicWorld(void)
{
    Q_ASSERT(m_btWorld == nullptr);

    m_btCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_btDispatcher = new btCollisionDispatcher(m_btCollisionConfiguration);
    m_btOverlappingPairCache = new btDbvtBroadphase();
    m_btSolver = new btSequentialImpulseConstraintSolver;

    m_btWorld = new btDiscreteDynamicsWorld(
                m_btDispatcher, m_btOverlappingPairCache, m_btSolver, m_btCollisionConfiguration);

    m_btWorld->setGravity(btVector3(m_gravity.x(), m_gravity.y(), m_gravity.z()));

    for (auto object: m_objects) {
        if (object->isEnabled())
            handleObjectAdded(object, false);
    }

    return true;
}

void World::releaseDynamicWorld(void)
{
    Q_ASSERT(m_btWorld != nullptr);

    delete m_btWorld;
    delete m_btSolver;
    delete m_btOverlappingPairCache;
    delete m_btDispatcher;
    delete m_btCollisionConfiguration;

    m_btWorld = nullptr;
    m_btSolver = nullptr;
    m_btOverlappingPairCache = nullptr;
    m_btDispatcher = nullptr;
    m_btCollisionConfiguration = nullptr;
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
