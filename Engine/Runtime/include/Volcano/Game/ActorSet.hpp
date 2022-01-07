//
//
#ifndef VOLCANO_GAME_ACTORSET_HPP
#define VOLCANO_GAME_ACTORSET_HPP

#include <QList>
#include <QString>
#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

using ActorList = QList<Actor *>;

class ActorSet: public Object {
    Q_OBJECT

public:
    ActorSet(QObject *parent = nullptr);
    ~ActorSet(void) override;

public:
    void tick(Duration elapsed) override;
    const QList<Actor *> &actors(void) const;

signals:
    void actorAdded(Actor *actor);
    void actorRemoved(Actor *actor);

protected:
    void appendActor(Actor *actor);
    qsizetype actorCount(void) const;
    Actor *actorAt(qsizetype index);
    void clearActors(void);
    void replaceActor(qsizetype index, Actor *actor);
    void removeLastActor(void);
    virtual void handleActorAdded(Actor *actor);
    virtual void handleActorRemoved(Actor *actor);

private:
    ActorList m_actorList;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ACTORSET_HPP
