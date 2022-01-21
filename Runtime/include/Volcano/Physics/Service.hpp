//
//
#ifndef VOLCANO_PHYSICS_SERVICE_HPP
#define VOLCANO_PHYSICS_SERVICE_HPP

#include <Volcano/Service.hpp>
#include <Volcano/Physics/Common.hpp>
#include <Volcano/Physics/Box.hpp>
#include <Volcano/Physics/Cylinder.hpp>
#include <Volcano/Physics/Plane.hpp>
#include <Volcano/Physics/Sphere.hpp>

VOLCANO_PHYSICS_BEGIN

class Service: public Volcano::Service {
    Q_OBJECT

public:
    Service(QObject *parent = nullptr);
    ~Service(void) override;

public:
    virtual void stepSimulation(Duration elapsed) = 0;
    virtual const QVector3D &gravity(void) const = 0;
    virtual void setGravity(const QVector3D &v) = 0;
    virtual Box *createBox(float length, float width, float height) = 0;
    virtual Cylinder *createCylinder(float radius, float height) = 0;
    virtual Plane *createPlane(float x, float y, float z, float d) = 0;
    virtual Sphere *createSphere(float radius) = 0;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_SERVICE_HPP
