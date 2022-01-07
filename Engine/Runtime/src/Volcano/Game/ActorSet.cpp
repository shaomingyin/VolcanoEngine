//
//
#include <Volcano/Game/ActorSet.hpp>

VOLCANO_GAME_BEGIN

ActorSet::ActorSet(QObject *parent):
    Object(parent)
{
}

ActorSet::~ActorSet(void)
{
}

void ActorSet::tick(Duration elapsed)
{
    for (auto actor: m_actorList)
        actor->tick(elapsed);
}

const QList<Actor *> &ActorSet::actors(void) const
{
    return m_actorList;
}

void ActorSet::appendActor(Actor *actor)
{
    m_actorList.append(actor);
    handleActorAdded(actor);
}

qsizetype ActorSet::actorCount(void) const
{
    return m_actorList.count();
}

Actor *ActorSet::actorAt(qsizetype index)
{
    if (0 <= index && index < m_actorList.size())
        return m_actorList.at(index);
    return nullptr;
}

void ActorSet::clearActors(void)
{
    QList<Actor *> actors = std::move(m_actorList);
    for (auto actor: actors)
        handleActorRemoved(actor);
}

void ActorSet::replaceActor(qsizetype index, Actor *actor)
{
    if (0 <= index && index < m_actorList.size()) {
        auto oldActor = m_actorList.at(index);
        m_actorList.replace(index, actor);
        handleActorRemoved(oldActor);
        handleActorAdded(actor);
    }
}

void ActorSet::removeLastActor(void)
{
    if (!m_actorList.isEmpty()) {
        auto actor = m_actorList.last();
        m_actorList.removeLast();
        handleActorRemoved(actor);
    }
}

void ActorSet::handleActorAdded(Actor *object)
{
    emit actorAdded(object);
}

void ActorSet::handleActorRemoved(Actor *object)
{
    emit actorRemoved(object);
}

VOLCANO_GAME_END
