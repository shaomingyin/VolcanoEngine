//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    Object(parent),
    m_gravity(0.0f, -9.8f, 0.0f)
{
}

World::~World(void)
{
}

bool World::isDynamic(void)
{
    return (physicsService() != nullptr);
}

const QVector3D &World::gravity(void) const
{
    return m_gravity;
}

void World::setGravity(const QVector3D &v)
{
    if (qFuzzyCompare(m_gravity, v))
        return;

    auto pService = physicsService();
    if (pService != nullptr) {
        m_gravity = v;
        pService->setGravity(v);
        emit gravityChanged(v);
    }
}

Light *World::ambientLight(void)
{
    return &m_ambientLight;
}

const ActorList &World::actors(void) const
{
    return m_actorList;
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

void World::appendActor(Actor *actor)
{
    m_actorList.append(actor);
    handleActorAdded(actor);
}

qsizetype World::actorCount(void) const
{
    return m_actorList.count();
}

Actor *World::actorAt(qsizetype index)
{
    if (0 <= index && index < m_actorList.size())
        return m_actorList.at(index);
    return nullptr;
}

void World::clearActors(void)
{
    QList<Actor *> actors = std::move(m_actorList);
    for (auto actor: actors)
        handleActorRemoved(actor);
}

void World::replaceActor(qsizetype index, Actor *actor)
{
    if (0 <= index && index < m_actorList.size()) {
        auto oldActor = m_actorList.at(index);
        m_actorList.replace(index, actor);
        handleActorRemoved(oldActor);
        handleActorAdded(actor);
    }
}

void World::removeLastActor(void)
{
    if (!m_actorList.isEmpty()) {
        auto actor = m_actorList.last();
        m_actorList.removeLast();
        handleActorRemoved(actor);
    }
}

void World::tick(void)
{
    for (auto actor: m_actorList)
        actor->updateState();
}

void World::draw(void)
{
    auto gService = graphicsService();

    gService->setAmbientLight(m_ambientLight.color());
    gService->resetTransform();

    for (auto &actor: m_actorList) {
        gService->pushTransform(true);
        actor->updateGraphics();
        gService->popTransform();
    }
}

void World::handleActorAdded(Actor *actor)
{
    actor->setParent(this);
    emit actorAdded(actor);
}

void World::handleActorRemoved(Actor *actor)
{
    actor->setParent(nullptr);
    emit actorRemoved(actor);
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
