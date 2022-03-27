//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>
#include <QString>
#include <QVector3D>
#include <QObject>
#include <QElapsedTimer>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Actor.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class World: public Object {
    Q_OBJECT
    Q_PROPERTY(bool dynamic READ isDynamic NOTIFY dynamicChanged)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(Light *ambientLight READ ambientLight)
    Q_PROPERTY(QQmlListProperty<Actor> actors READ qmlActors)
    Q_CLASSINFO("DefaultProperty", "actors")

public:
    World(QObject *parent = nullptr);
    World(Context *context, QObject *parent = nullptr);
    ~World(void) override;

public:
    bool isDynamic(void);
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &v);
    Light *ambientLight(void);
    const ActorList &actors(void) const;
    QQmlListProperty<Actor> qmlActors(void);

signals:
    void dynamicChanged(bool v);
    void gravityChanged(const QVector3D &v);
    void actorAdded(Actor *actor);
    void actorRemoved(Actor *actor);

protected:
    void appendActor(Actor *actor);
    qsizetype actorCount(void) const;
    Actor *actorAt(qsizetype index);
    void clearActors(void);
    void replaceActor(qsizetype index, Actor *actor);
    void removeLastActor(void);
    void tick(void) override;
    void draw(void) override;
    virtual void handleActorAdded(Actor *actor);
    virtual void handleActorRemoved(Actor *actor);

private:
    static void qmlAppendActor(QQmlListProperty<Actor> *list, Actor *actor);
    static qsizetype qmlActorCount(QQmlListProperty<Actor> *list);
    static Actor *qmlActorAt(QQmlListProperty<Actor> *list, qsizetype index);
    static void qmlClearActors(QQmlListProperty<Actor> *list);
    static void qmlReplaceActor(QQmlListProperty<Actor> *list, qsizetype index, Actor *actor);
    static void qmlRemoveLastActor(QQmlListProperty<Actor> *list);

private:
    QVector3D m_gravity;
    Light m_ambientLight;
    ActorList m_actorList;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
