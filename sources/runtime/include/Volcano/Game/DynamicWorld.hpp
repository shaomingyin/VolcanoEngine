//
//
#ifndef VOLCANO_GAME_DYNAMICWORLD_HPP
#define VOLCANO_GAME_DYNAMICWORLD_HPP

#include <QVector3D>

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class DynamicWorld: public World {
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)

public:
    DynamicWorld(QObject *parent = nullptr);
    ~DynamicWorld(void) override;

public:
    void tick(float elapsed) override;
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &v);

protected:
    void handleObjectAdded(Object *object, bool emitSignal) override;
    void handleObjectRemoved(Object *object, bool emitSignal) override;

signals:
    void gravityChanged(const QVector3D &v);

private:
    bool init(void);
    void release(void);

private:
    btDefaultCollisionConfiguration *m_btCollisionConfiguration;
    btCollisionDispatcher *m_btDispatcher;
    btBroadphaseInterface *m_btOverlappingPairCache;
    btSequentialImpulseConstraintSolver *m_btSolver;
    btDiscreteDynamicsWorld *m_btWorld;
    QVector3D m_gravity;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DYNAMICWORLD_HPP
