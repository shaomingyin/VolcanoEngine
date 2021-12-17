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
    WorldBase::handleActorAdded(actor);
}

void World::handleActorRemoved(Actor *actor)
{
    actor->setParent(nullptr);
    WorldBase::handleActorRemoved(actor);
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
