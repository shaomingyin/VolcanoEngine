//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>
#include <QString>
#include <QObject>
#include <QElapsedTimer>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/WorldBase.hpp>

VOLCANO_GAME_BEGIN

class World: public WorldBase {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Actor> actors READ qmlActors)
    Q_CLASSINFO("DefaultProperty", "actors")

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    QQmlListProperty<Actor> qmlActors(void);

protected:
    void handleActorAdded(Actor *actor) override;
    void handleActorRemoved(Actor *actor) override;

private:
    static void qmlAppendActor(QQmlListProperty<Actor> *list, Actor *actor);
    static qsizetype qmlActorCount(QQmlListProperty<Actor> *list);
    static Actor *qmlActorAt(QQmlListProperty<Actor> *list, qsizetype index);
    static void qmlClearActors(QQmlListProperty<Actor> *list);
    static void qmlReplaceActor(QQmlListProperty<Actor> *list, qsizetype index, Actor *actor);
    static void qmlRemoveLastActor(QQmlListProperty<Actor> *list);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
