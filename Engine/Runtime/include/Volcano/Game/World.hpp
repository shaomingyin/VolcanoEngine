//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>
#include <QString>
#include <QObject>
#include <QElapsedTimer>
#include <QQmlListProperty>

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/ActorSet.hpp>

VOLCANO_GAME_BEGIN

class World: public ActorSet {
    Q_OBJECT
    Q_PROPERTY(bool dynamic READ isDynamic WRITE setDynamic NOTIFY dynamicChanged)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(QQmlListProperty<Actor> actors READ qmlActors)
    Q_CLASSINFO("DefaultProperty", "actors")

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    void tick(Duration elapsed) override;
    bool isDynamic(void) const;
    void setDynamic(bool v);
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &v);
    QQmlListProperty<Actor> qmlActors(void);

signals:
    void dynamicChanged(bool v);
    void gravityChanged(const QVector3D &v);

protected:
    void handleActorAdded(Actor *actor) override;
    void handleActorRemoved(Actor *actor) override;

private:
    bool enalbeDynamic(void);
    void disableDynamic(void);
    static void qmlAppendActor(QQmlListProperty<Actor> *list, Actor *actor);
    static qsizetype qmlActorCount(QQmlListProperty<Actor> *list);
    static Actor *qmlActorAt(QQmlListProperty<Actor> *list, qsizetype index);
    static void qmlClearActors(QQmlListProperty<Actor> *list);
    static void qmlReplaceActor(QQmlListProperty<Actor> *list, qsizetype index, Actor *actor);
    static void qmlRemoveLastActor(QQmlListProperty<Actor> *list);

private:
    btDefaultCollisionConfiguration *m_btCollisionConfiguration;
    btCollisionDispatcher *m_btDispatcher;
    btBroadphaseInterface *m_btOverlappingPairCache;
    btSequentialImpulseConstraintSolver *m_btSolver;
    btDiscreteDynamicsWorld *m_btWorld;
    QVector3D m_gravity;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
