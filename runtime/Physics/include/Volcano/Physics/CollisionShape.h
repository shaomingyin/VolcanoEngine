//
//
#ifndef VOLCANO_PHYSICS_COLLISIONSHAPE_H
#define VOLCANO_PHYSICS_COLLISIONSHAPE_H

#include <QObject>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

class CollisionShape: public QObject {
    Q_OBJECT

public:
    explicit CollisionShape(QObject* parent = nullptr);
    ~CollisionShape() override;

public:
    virtual bool isValid() const noexcept = 0;

signals:
    void shapeChanged();

private:
    friend class System;

    JPH::ShapeRefC createShape() const;
    virtual const JPH::ShapeSettings& settings() const noexcept = 0;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_COLLISIONSHAPE_H
