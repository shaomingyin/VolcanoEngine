//
//
#ifndef VOLCANO_WORLD_SPHERECOLLISIONSHAPE_H
#define VOLCANO_WORLD_SPHERECOLLISIONSHAPE_H

#include <memory>

#include <Volcano/World/Common.h>
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

class SphereCollisionShape: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)

public:
    SphereCollisionShape(QObject* parent = nullptr);

public:
    float radius() const {
        return radius_;
    }

    void setRadius(float v);
    void componentComplete() override;

signals:
    void radiusChanged(float v);

private:
    float radius_;
    std::unique_ptr<btSphereShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SPHERECOLLISIONSHAPE_H
