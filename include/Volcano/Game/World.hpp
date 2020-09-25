//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>
#include <QVector3D>
#include <QObject>

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
typedef QList<Camera *> CameraList;

class VOLCANO_API World: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(Light *ambientLight READ ambientLight)

public:
    enum
    {
        InitPhysics = 0x1
    };

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    bool init(int flags);
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &r);
    Light *ambientLight(void);
    void addEntity(Entity *p);
    void removeEntity(Entity *p);
    virtual void stepSimulation(float elapsed);
    //virtual void buildSnapshot(Snapshot &r);

signals:
    void gravityChanged(const QVector3D &r);
    void entityAdded(Entity *p);
    void entityRemoved(Entity *p);

protected:
    //void buildView(View &r);

protected:
    LightList m_lightList;
    EntityList m_entityList;
    CameraList m_cameraList;

private:
    Light m_ambientLight;
    QVector3D m_gravity;
    btDynamicsWorld *m_btWorld;
};

VOLCANO_INLINE const QVector3D &World::gravity(void) const
{
    return m_gravity;
}

VOLCANO_INLINE Light *World::ambientLight(void)
{
    return &m_ambientLight;
}

VOLCANO_INLINE void World::addEntity(Entity *p)
{
    Q_ASSERT(!m_entityList.contains(p));
    m_entityList.append(p);
    entityAdded(p);
}

VOLCANO_INLINE void World::removeEntity(Entity *p)
{
    Q_ASSERT(m_entityList.contains(p));
    m_entityList.removeAll(p);
    entityRemoved(p);
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
