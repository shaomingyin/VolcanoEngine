//
//
#include <memory>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Collision.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    ActorSet(parent),
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
        disableDynamic();
}

void World::tick(Duration elapsed)
{
    if (Q_LIKELY(m_btWorld != nullptr)) {
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
        m_btWorld->stepSimulation(float(ms.count()) / 1000000.0f);
        // TODO
    }

    ActorSet::tick(elapsed);
}

bool World::isDynamic(void) const
{
    return (m_btWorld != nullptr);
}

void World::setDynamic(bool v)
{
    if (isDynamic()) {
        if (!v)
            disableDynamic();
    } else {
        if (v)
            enalbeDynamic();
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

QQmlListProperty<Actor> World::qmlActors(void)
{
    return { this, this,
        &World::qmlAppendActor,
        &World::qmlActorCount,
        &World::qmlActorAt,
        &World::qmlClearActors,
        &World::qmlReplaceActor,
        &World::qmlRemoveLastActor };
}

void World::handleActorAdded(Actor *actor)
{
    actor->setParent(this);
    ActorSet::handleActorAdded(actor);

    auto entity = qobject_cast<Entity *>(actor);
    if (entity == nullptr)
        return;

    for (auto comp: entity->components()) {
        auto collision = qobject_cast<Collision *>(comp);
        if (collision == nullptr)
            continue;
        // TODO
        // collision->shape()
    }
}

void World::handleActorRemoved(Actor *actor)
{
    actor->setParent(nullptr);
    ActorSet::handleActorRemoved(actor);
}

bool World::enalbeDynamic(void)
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

void World::disableDynamic(void)
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

void World::qmlAppendActor(QQmlListProperty<Actor> *list, Actor *actor)
{
    reinterpret_cast<World *>(list->data)->appendActor(actor);
}

qsizetype World::qmlActorCount(QQmlListProperty<Actor> *list)
{
    return reinterpret_cast<World *>(list->data)->actorCount();
}

Actor *World::qmlActorAt(QQmlListProperty<Actor> *list, qsizetype i)
{
    return reinterpret_cast<World *>(list->data)->actorAt(i);
}

void World::qmlClearActors(QQmlListProperty<Actor> *list)
{
    reinterpret_cast<World *>(list->data)->clearActors();
}

void World::qmlReplaceActor(QQmlListProperty<Actor> *list, qsizetype i, Actor *actor)
{
    reinterpret_cast<World *>(list->data)->replaceActor(i, actor);
}

void World::qmlRemoveLastActor(QQmlListProperty<Actor> *list)
{
    reinterpret_cast<World *>(list->data)->removeLastActor();
}

VOLCANO_GAME_END
