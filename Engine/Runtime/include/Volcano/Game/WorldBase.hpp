//
//
#ifndef VOLCANO_GAME_WORLDBASE_HPP
#define VOLCANO_GAME_WORLDBASE_HPP

#include <QList>
#include <QString>
#include <QColor>
#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Actor.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

using ActorList = QList<Actor *>;

class WorldBase: public Object {
    Q_OBJECT

public:
    WorldBase(QObject *parent = nullptr);
    ~WorldBase(void) override;

public:
    const QColor ambientLight(void) const;
    void tick(Duration elapsed) override;
    const QList<Actor *> &actors(void) const;

signals:
    void actorAdded(Actor *actor);
    void actorRemoved(Actor *actor);

protected:
    void setAmbientLight(const QColor &v);
    void appendActor(Actor *actor);
    qsizetype actorCount(void) const;
    Actor *actorAt(qsizetype index);
    void clearActors(void);
    void replaceActor(qsizetype index, Actor *actor);
    void removeLastActor(void);
    virtual void handleActorAdded(Actor *actor);
    virtual void handleActorRemoved(Actor *actor);

private:
    QColor m_ambientLight;
    ActorList m_actorList;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLDBASE_HPP
