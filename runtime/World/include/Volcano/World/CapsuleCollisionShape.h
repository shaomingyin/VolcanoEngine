//
//
#ifndef VOLCANO_WORLD_CAPSULECOLLISIONSHAPE_H
#define VOLCANO_WORLD_CAPSULECOLLISIONSHAPE_H

#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

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
    float radius_ = 0.5f;
    float cylinderHeight_ = 1.0f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAPSULECOLLISIONSHAPE_H
