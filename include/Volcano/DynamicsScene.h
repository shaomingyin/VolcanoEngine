//
//
#ifndef VOLCANO_SCENE_H
#define VOLCANO_SCENE_H

#include <QVector>
#include <QObject>
#include <QVector3D>
#include <QScopedPointer>

#include <btBulletDynamicsCommon.h>

#include <Volcano/Common.h>
#include <Volcano/Object.h>
#include <Volcano/Camera.h>
#include <Volcano/Entity.h>
#include <Volcano/Light.h>
#include <Volcano/View.h>

VOLCANO_BEGIN

class VOLCANO_API Scene: public Object
{
    Q_OBJECT
    Q_PROPERTY(bool physics READ isPhysicsEnabled WRITE setPhysicsEnabled NOTIFY physicsChanged)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)

public:
    Scene(QObject *parent = nullptr);
    ~Scene(void) override;

public:
    virtual void update(float elapsed);
    void buildView(Camera &cam, View &view);
    bool isPhysicsEnabled(void) const;
    void setPhysicsEnabled(bool v);
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &v);

signals:
    void physicsChanged(bool v);
    void gravityChanged(const QVector3D &v);

private:
    QScopedPointer<btDefaultCollisionConfiguration> m_btcc;
    QScopedPointer<btCollisionDispatcher> m_btcd;
    QScopedPointer<btBroadphaseInterface> m_btbi;
    QScopedPointer<btSequentialImpulseConstraintSolver> m_btsics;
    QScopedPointer<btDiscreteDynamicsWorld> m_btWorld;
    QVector3D m_gravity;
};

VOLCANO_INLINE bool Scene::isPhysicsEnabled(void) const
{
    return m_btWorld;
}

VOLCANO_INLINE const QVector3D &Scene::gravity(void) const
{
    return m_gravity;
}

VOLCANO_INLINE void Scene::setGravity(const QVector3D &v)
{
    if (m_gravity != v)
    {
        m_gravity = v;
        gravityChanged(v);
    }
}

VOLCANO_END

#endif // VOLCANO_SCENE_H
