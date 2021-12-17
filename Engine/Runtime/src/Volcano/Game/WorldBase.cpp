//
//
#include <Volcano/Game/WorldBase.hpp>

VOLCANO_GAME_BEGIN

WorldBase::WorldBase(QObject *parent):
    Object(parent)
{
}

WorldBase::~WorldBase(void)
{
}

void WorldBase::tick(Duration elapsed)
{
    for (auto actor: m_actorList)
        actor->tick(elapsed);
}

const QList<Actor *> &WorldBase::actors(void) const
{
    return m_actorList;
}

void WorldBase::appendActor(Actor *actor)
{
    m_actorList.append(actor);
    handleActorAdded(actor);
}

qsizetype WorldBase::actorCount(void) const
{
    return m_actorList.count();
}

Actor *WorldBase::actorAt(qsizetype index)
{
    if (0 <= index && index < m_actorList.size())
        return m_actorList.at(index);
    return nullptr;
}

void WorldBase::clearActors(void)
{
    QList<Actor *> actors = std::move(m_actorList);
    for (auto actor: actors)
        handleActorRemoved(actor);
}

void WorldBase::replaceActor(qsizetype index, Actor *actor)
{
    if (0 <= index && index < m_actorList.size()) {
        auto oldActor = m_actorList.at(index);
        m_actorList.replace(index, actor);
        handleActorRemoved(oldActor);
        handleActorAdded(actor);
    }
}

void WorldBase::removeLastActor(void)
{
    if (!m_actorList.isEmpty()) {
        auto actor = m_actorList.last();
        m_actorList.removeLast();
        handleActorRemoved(actor);
    }
}

void WorldBase::handleActorAdded(Actor *object)
{
    emit actorAdded(object);
}

void WorldBase::handleActorRemoved(Actor *object)
{
    emit actorRemoved(object);
}

VOLCANO_GAME_END
