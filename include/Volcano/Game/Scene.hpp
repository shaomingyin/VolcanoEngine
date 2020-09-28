//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <QList>
#include <QVector3D>
#include <QObject>
#include <QQmlListProperty>

#define register
#include <btBulletDynamicsCommon.h>
#undef register

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

typedef QList<Light *> LightList;
typedef QList<Entity *> EntityList;

class VOLCANO_API Scene: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(Light *ambientLight READ ambientLight)
    Q_PROPERTY(QQmlListProperty<Volcano::Game::Light> lights READ lights)
    Q_PROPERTY(QQmlListProperty<Volcano::Game::Entity> entities READ entities)

public:
    Scene(QObject *parent = nullptr);
    ~Scene(void) override;

public:
    virtual bool init(void);
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &r);
    Light *ambientLight(void);
    QQmlListProperty<Light> lights(void);
    QQmlListProperty<Entity> entities(void);
    virtual void update(float elapsed);
    //virtual void buildSnapshot(Snapshot &r);

signals:
    void gravityChanged(const QVector3D &r);
    void entityAdded(Entity *p);
    void entityRemoved(Entity *p);

private:
    static void appendLight(QQmlListProperty<Volcano::Game::Light> *p, Light *n);
    static int lightCount(QQmlListProperty<Volcano::Game::Light> *p);
    static Light *lightAt(QQmlListProperty<Volcano::Game::Light> *p, int i);
    static void clearLight(QQmlListProperty<Volcano::Game::Light> *p);
    static void replaceLight(QQmlListProperty<Volcano::Game::Light> *p, int i, Light *n);
    static void removeLastLight(QQmlListProperty<Volcano::Game::Light> *p);

    static void appendEntity(QQmlListProperty<Volcano::Game::Entity> *p, Entity *n);
    static int entityCount(QQmlListProperty<Volcano::Game::Entity> *p);
    static Entity *entityAt(QQmlListProperty<Volcano::Game::Entity> *p, int i);
    static void clearEntity(QQmlListProperty<Volcano::Game::Entity> *p);
    static void replaceEntity(QQmlListProperty<Volcano::Game::Entity> *p, int i, Entity *n);
    static void removeLastEntity(QQmlListProperty<Volcano::Game::Entity> *p);

protected:
    LightList m_lightList;
    EntityList m_entityList;

private:
    Light m_ambientLight;
    QVector3D m_gravity;
    btDynamicsWorld *m_btWorld;
};

VOLCANO_INLINE const QVector3D &Scene::gravity(void) const
{
    return m_gravity;
}

VOLCANO_INLINE Light *Scene::ambientLight(void)
{
    return &m_ambientLight;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_HPP
