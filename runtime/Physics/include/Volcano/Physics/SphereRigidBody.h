//
//
#ifndef VOLCANO_PHYSICS_SPHERERIGIDBODY_H
#define VOLCANO_PHYSICS_SPHERERIGIDBODY_H

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

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

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_SPHERERIGIDBODY_H
