//
//
#include <memory>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    QObject(parent),
    m_state(State::Empty),
    m_loadingProgress(0.0f),
    m_frameTimer(-1),
    m_frameCountTimer(-1),
    m_btCollisionConfiguration(nullptr),
    m_btDispatcher(nullptr),
    m_btOverlappingPairCache(nullptr),
    m_btSolver(nullptr),
    m_btWorld(nullptr),
    m_gravity(0.0f, -9.8f, 0.0f)
{
    setFpsMax(20);
}

World::~World(void)
{
    if (m_btWorld != nullptr)
        releaseDynamicWorld();
}

World::State World::state(void) const
{
    return m_state;
}

qreal World::loadingProgress(void) const
{
    qreal ret;

    switch (m_state) {
    case State::Ready:
        ret = 1.0f;
        break;
    case State::Loading:
        ret = m_loadingProgress;
        break;
    default:
        ret = 0.0f;
        break;
    }

    return ret;
}

int World::fps(void) const
{
    return m_frameCountPerSecond;
}

int World::fpsMax(void) const
{
    Q_ASSERT(m_elapsedMin > 0);

    return (1000 / m_elapsedMin);
}

void World::setFpsMax(int v)
{
    int tmp = v;
    if (tmp <= 0)
        tmp = 1;

    m_frameCount = 0;
    m_frameCountPerSecond = 0;

    if (m_frameTimer >= 0) {
        killTimer(m_frameTimer);
        m_frameTimer = -1;
    }

    m_elapsedMin = 1000 / tmp;

    if (m_elapsedTimer.isValid())
        m_elapsedTimer.start();
    else
        m_elapsedTimer.restart();

    m_frameTimer = startTimer(m_elapsedMin, Qt::PreciseTimer);

    if (m_frameCountTimer >= 0) {
        killTimer(m_frameCountTimer);
        m_frameCountTimer = -1;
    }

    m_frameCountTimer = startTimer(1000);

    emit fpsMaxChanged(tmp);
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

qsizetype World::objectCount(void)
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

void World::timerEvent(QTimerEvent *event)
{
    auto id = event->timerId();
    if (id == m_frameTimer) {
        auto elapsed = m_elapsedTimer.restart();
        frame(float(elapsed) / 1000.0f);
        return;
    }

    if (id == m_frameCountTimer) {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        return;
    }
}

void World::setState(State v)
{
    if (m_state != v) {
        m_state = v;
        emit stateChanged(v);
    }
}

void World::setLoadingProgress(qreal v)
{
    Q_ASSERT(m_state == State::Loading);

    auto tmp = qBound(0.0f, v, 1.0f);

    if (!qFuzzyCompare(m_loadingProgress, tmp)) {
        m_loadingProgress = tmp;
        emit loadingProgressChanged(tmp);
    }
}

void World::frame(float elapsed)
{
    for (auto object: m_objects)
        object->update(elapsed);

    if (m_btWorld != nullptr) {
        m_btWorld->stepSimulation(elapsed);
        // TODO
    }

    m_frameCount += 1;
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
