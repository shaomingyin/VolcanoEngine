//
//
#ifndef VOLCANO_WORLD_SPHERERIGIDBODY_H
#define VOLCANO_WORLD_SPHERERIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class SphereRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)

public:
    SphereRigidBody(QObject* parent = nullptr);

public:
    float radius() const {
        return shape_.getImplicitShapeDimensions().x();
    }

    void setRadius(float v) {
        preCollisionShapeChange();
        auto tmp = shape_.getImplicitShapeDimensions();
        shape_.setImplicitShapeDimensions({ v, tmp.y(), tmp.z() });
        postCollisionShapeChange();
        emit radiusChanged(v);
    }

signals:
    void radiusChanged(float v);

private:
	btSphereShape shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SPHERERIGIDBODY_H
