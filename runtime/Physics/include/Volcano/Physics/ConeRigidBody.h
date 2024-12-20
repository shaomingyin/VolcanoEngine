//
//
#ifndef VOLCANO_PHYSICS_CONERIGIDBODY_H
#define VOLCANO_PHYSICS_CONERIGIDBODY_H

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class ConeRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    ConeRigidBody(QObject* parent = nullptr);

    float radius() const {
        return shape_.getRadius();
    }

    void setRadius(float v) {
        preCollisionShapeChange();
        shape_.setRadius(v);
        postCollisionShapeChange();
        emit radiusChanged(v);
    }

    float height() const {
        return shape_.getHeight();
    }

    void setHeight(float v) {
        preCollisionShapeChange();
        shape_.setHeight(v);
        postCollisionShapeChange();
        emit heightChanged(v);
    }

signals:
    void radiusChanged(float v);
    void heightChanged(float v);

private:
	btConeShape shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_CONERIGIDBODY_H
