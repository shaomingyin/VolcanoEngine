//
//
#ifndef VOLCANO_PHYSICS_RIGIDBODY_H
#define VOLCANO_PHYSICS_RIGIDBODY_H

#include <QVector3D>

#include <Volcano/World/Transformable.h>
#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

class RigidBody: public World::Transformable, public btRigidBody {
    Q_OBJECT
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(QVector3D scaling READ scaling WRITE setScaling NOTIFY scalingChanged FINAL)

public:
    RigidBody(QObject* parent = nullptr);
    ~RigidBody() override;

public:
    float mass() const {
        return getMass();
    }

    void setMass(float v) {
        setMassProps(v, { 0.0f, 0.0f, 0.0f });
    }

    QVector3D scaling() const {
        const btVector3& r = getCollisionShape()->getLocalScaling();
        return { r.x(), r.y(), r.z() };
    }

    void setScaling(const QVector3D& v) {
        getCollisionShape()->setLocalScaling({ v.x(), v.y(), v.z() });
        emit scalingChanged(v);
    }

    btDynamicsWorld* ownerWorld() {
        return owner_world_;
    }

    void setOwnerWorld(btDynamicsWorld* p) {
        owner_world_ = p;
    }

signals:
    void massChanged(float v);
    void scalingChanged(const QVector3D& v);

protected:
    void preCollisionShapeChange() {
        if (owner_world_ != nullptr) {
            owner_world_->removeRigidBody(this);
        }
    }

    void postCollisionShapeChange() {
        if (owner_world_ != nullptr) {
            owner_world_->addRigidBody(this);
            // cleanProxyFromPairs?
        }
    }

private:
    class MotionState: public btMotionState {
    public:
        MotionState(World::Transformable& orig);

    public:
        void getWorldTransform(btTransform& world_transform) const override;
        void setWorldTransform(const btTransform& world_transform) override;

    private:
        World::Transformable& orig_;
    };

private:
    btDynamicsWorld* owner_world_;
    MotionState motion_state_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_RIGIDBODY_H
