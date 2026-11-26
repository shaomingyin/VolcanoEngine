//
//
#ifndef VOLCANO_PHYSICS_CAPSULECOLLISIONSHAPE_H
#define VOLCANO_PHYSICS_CAPSULECOLLISIONSHAPE_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

#include <Volcano/Physics/CollisionShape.h>

VOLCANO_PHYSICS_BEGIN

class CapsuleCollisionShape: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(float cylinderHeight READ cylinderHeight WRITE setCylinderHeight NOTIFY cylinderHeightChanged)
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)

public:
    explicit CapsuleCollisionShape(QObject* parent = nullptr);

public:
    float radius() const noexcept;
    void setRadius(float radius);
    float cylinderHeight() const noexcept;
    void setCylinderHeight(float height);
    bool isValid() const noexcept override;

signals:
    void radiusChanged(float radius);
    void cylinderHeightChanged(float height);
    void validChanged(bool valid);

private:
    const JPH::CapsuleShapeSettings& settings() const noexcept override;

private:
    JPH::CapsuleShapeSettings settings_ { 0.5f, 0.5f };
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_CAPSULECOLLISIONSHAPE_H
