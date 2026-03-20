//
//
#ifndef VOLCANO_WORLD_COLLISIONSHAPE_H
#define VOLCANO_WORLD_COLLISIONSHAPE_H

#include <QObject>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class CollisionShape: public QObject {
    Q_OBJECT
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)

public:
    CollisionShape(QObject* parent = nullptr);
    ~CollisionShape() override;

public:
    float mass() const noexcept;
    void setMass(float v) noexcept;

signals:
    void massChanged(float v);

protected:
    void setShape(JPH::Shape* p) noexcept;

private:
    JPH::Shape* shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COLLISIONSHAPE_H
