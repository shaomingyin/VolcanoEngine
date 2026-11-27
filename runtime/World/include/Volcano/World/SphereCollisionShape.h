//
//
#ifndef VOLCANO_WORLD_SPHERECOLLISIONSHAPE_H
#define VOLCANO_WORLD_SPHERECOLLISIONSHAPE_H

#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

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
    float radius_ = 0.5f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SPHERECOLLISIONSHAPE_H
