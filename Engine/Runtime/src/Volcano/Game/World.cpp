//
//
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
        releaseDynamic();
}

void World::tick(float elapsed)
{
    if (Q_LIKELY(m_btWorld != nullptr)) {
        m_btWorld->stepSimulation(elapsed);
        // TODO
    }

    for (auto object: m_objects)
        object->tick(elapsed);
}

bool World::isDynamic(void) const
{
    return  (m_btWorld != nullptr);
}

void World::setDynamic(bool v)
{
    if (m_btWorld != nullptr) {
        if (!v)
            releaseDynamic();
    } else {
        if (v)
            initDynamic();
    }
}

const QVector3D &World::gravity(void) const
{
    return m_gravity;
}

void World::setGravity(const QVector3D &v)
{
    if (qFuzzyCompare(m_gravity, v))
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
    if (emitSignal)
        emit objectAdded(object);
}

void World::handleObjectRemoved(Object *object, bool emitSignal)
{
    object->setParent(nullptr);
    if (emitSignal)
        emit objectRemoved(object);
}

bool World::initDynamic(void)
{
    Q_ASSERT(m_btWorld == nullptr);

    auto btCollisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    if (!btCollisionConfiguration)
        return false;

    auto btDispatcher = std::make_unique<btCollisionDispatcher>(btCollisionConfiguration.get());
    if (!btDispatcher)
        return false;

    auto btOverlappingPairCache = std::make_unique<btDbvtBroadphase>();
    if (!btOverlappingPairCache)
        return false;

    auto btSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
    if (!btSolver)
        return false;

    auto btWorld = std::make_unique<btDiscreteDynamicsWorld>(
                btDispatcher.get(),
                btOverlappingPairCache.get(),
                btSolver.get(),
                btCollisionConfiguration.get());

    if (!btWorld)
        return false;

    btWorld->setGravity(btVector3(m_gravity.x(), m_gravity.y(), m_gravity.z()));

    m_btCollisionConfiguration = btCollisionConfiguration.release();
    m_btDispatcher = btDispatcher.release();
    m_btOverlappingPairCache = btOverlappingPairCache.release();
    m_btSolver = btSolver.release();
    m_btWorld = btWorld.release();

    emit dynamicChanged(true);

    return true;
}

void World::releaseDynamic(void)
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

    emit dynamicChanged(false);
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
