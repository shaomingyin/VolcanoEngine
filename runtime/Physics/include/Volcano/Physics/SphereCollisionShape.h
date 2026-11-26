//
//
#ifndef VOLCANO_PHYSICS_SPHERECOLLISIONSHAPE_H
#define VOLCANO_PHYSICS_SPHERECOLLISIONSHAPE_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Volcano/Physics/CollisionShape.h>

VOLCANO_PHYSICS_BEGIN

class SphereCollisionShape: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)

public:
    explicit SphereCollisionShape(QObject* parent = nullptr);

public:
    float radius() const noexcept;
    void setRadius(float radius);
    bool isValid() const noexcept override;

signals:
    void radiusChanged(float radius);
    void validChanged(bool valid);

private:
    const JPH::SphereShapeSettings& settings() const noexcept override;

private:
    JPH::SphereShapeSettings settings_ { 0.5f };
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_SPHERECOLLISIONSHAPE_H
