//
//
#ifndef VOLCANO_PHYSICS_BULLET_SERVICE_HPP
#define VOLCANO_PHYSICS_BULLET_SERVICE_HPP

#include <QList>
#include <QString>
#include <QVector3D>
#include <QObject>
#include <QElapsedTimer>
#include <QQmlListProperty>

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Physics/Service.hpp>
#include <Volcano/Physics/Bullet/Common.hpp>

VOLCANO_PHYSICS_BULLET_BEGIN

class Service: public Volcano::Physics::Service {
    Q_OBJECT

public:
    Service(QObject *parent = nullptr);
    ~Service(void) override;

public:
    void stepSimulation(Duration elapsed) override;
    const QVector3D &gravity(void) const override;
    void setGravity(const QVector3D &v) override;
    Box *createBox(float length, float width, float height) override;
    void updateBox(Box *p) override;
    Cylinder *createCylinder(float radius, float height) override;
    void updateCylinder(Cylinder *p) override;
    Plane *createPlane(float x, float y, float z, float d) override;
    void updatePlane(Plane *p) override;
    Sphere *createSphere(float radius) override;
    void updateSphere(Sphere *p) override;

protected:
    bool onStart(void) override;
    void onStop(void) override;

private:
    btDefaultCollisionConfiguration *m_btCollisionConfiguration;
    btCollisionDispatcher *m_btDispatcher;
    btBroadphaseInterface *m_btOverlappingPairCache;
    btSequentialImpulseConstraintSolver *m_btSolver;
    btDiscreteDynamicsWorld *m_btWorld;
    QVector3D m_gravity;
};

VOLCANO_PHYSICS_BULLET_END

#endif // VOLCANO_PHYSICS_BULLET_SERVICE_HPP
