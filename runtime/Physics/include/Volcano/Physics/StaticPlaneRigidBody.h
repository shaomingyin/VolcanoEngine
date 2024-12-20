//
//
#ifndef VOLCANO_PHYSICS_STATICPLANERIGIDBODY_H
#define VOLCANO_PHYSICS_STATICPLANERIGIDBODY_H

#include <memory>

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class StaticPlaneRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(QVector3D normal READ normal WRITE setNormal NOTIFY normalChanged FINAL)
    Q_PROPERTY(float constant READ constant WRITE setConstant NOTIFY constantChanged FINAL)

public:
    StaticPlaneRigidBody(QObject* parent = nullptr);

public:
    QVector3D normal() const {
        const btVector3& r = shape_->getPlaneNormal();
        return { r.x(), r.y(), r.z() };
    }

    void setNormal(const QVector3D& v) {
        preCollisionShapeChange();
        float c = shape_->getPlaneConstant();
        shape_.reset(new btStaticPlaneShape({ v.x(), v.y(), v.z() }, c));
        postCollisionShapeChange();
        emit normalChanged(v);
    }

    float constant() const {
        return shape_->getPlaneConstant();
    }

    void setConstant(float v) {
        preCollisionShapeChange();
        btVector3 n = shape_->getPlaneNormal();
        shape_.reset(new btStaticPlaneShape(n, v));
        postCollisionShapeChange();
        emit constantChanged(v);
    }

signals:
    void normalChanged(const QVector3D& v);
    void constantChanged(float v);

private:
    std::unique_ptr<btStaticPlaneShape> shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_STATICPLANERIGIDBODY_H
