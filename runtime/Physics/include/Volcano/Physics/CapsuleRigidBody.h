//
//
#ifndef VOLCANO_PHYSICS_CAPSULERIGIDBODY_H
#define VOLCANO_PHYSICS_CAPSULERIGIDBODY_H

#include <memory>

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class CapsuleRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    CapsuleRigidBody(QObject* parent = nullptr);

public:
    float radius() const {
        return shape_->getRadius();
    }

    void setRadius(float v) {
        preCollisionShapeChange();
        float h = height();
        shape_.reset(new btCapsuleShape({ v, h }));
        setCollisionShape(shape_.get());
        postCollisionShapeChange();
        emit radiusChanged(v);
    }

    float height() const {
        return (shape_->getHalfHeight() * 2.0f);
    }

    void setHeight(float v) {
        preCollisionShapeChange();
        float r = shape_->getRadius();
        shape_.reset(new btCapsuleShape({ r, v }));
        setCollisionShape(shape_.get());
        postCollisionShapeChange();
        emit heightChanged(v);
    }

signals:
    void radiusChanged(float v);
    void heightChanged(float v);

private:
    std::unique_ptr<btCapsuleShape> shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_CAPSULERIGIDBODY_H
