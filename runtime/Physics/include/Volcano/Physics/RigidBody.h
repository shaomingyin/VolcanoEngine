//
//
#ifndef VOLCANO_WORLD_RIGIDBODY_H
#define VOLCANO_WORLD_RIGIDBODY_H

#include <QObject>

#include <Volcano/World/Common.h>
#include <Volcano/World/Shape.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class RigidBody: public Transformable {
    Q_OBJECT
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(Shape* shape READ shape WRITE setShape NOTIFY shapeChanged)

public:
    RigidBody(QObject* parent = nullptr);

public:
    float mass() noexcept;
    void setMass(float v) noexcept;
    Shape* shape() noexcept;
    void setShape(Shape* p) noexcept;

signals:
    void massChanged(float v);
    void shapeChanged(Shape* p);

private:
    float mass_;
    Shape* shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_RIGIDBODY_H
