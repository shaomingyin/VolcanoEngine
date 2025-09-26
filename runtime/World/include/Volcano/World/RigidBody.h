//
//
#ifndef VOLCANO_WORLD_RIGIDBODY_H
#define VOLCANO_WORLD_RIGIDBODY_H

#include <QQmlParserStatus>

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class RigidBody: public Transformable, public QQmlParserStatus {
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(Vector3 scaling READ scaling WRITE setScaling NOTIFY scalingChanged FINAL)

public:
    RigidBody(QObject* parent = nullptr);
    ~RigidBody() override;

public:
    float mass() const {
        return rigid_body_.getMass();
    }

    void setMass(float v) {
        // TODO
    }

    Vector3 scaling() const {
        const btVector3& r = rigid_body_.getCollisionShape()->getLocalScaling();
        return { r.x(), r.y(), r.z() };
    }

    void setScaling(const Vector3& v) {
        rigid_body_.getCollisionShape()->setLocalScaling({ v.x(), v.y(), v.z() });
        emit scalingChanged(v);
    }

    btDynamicsWorld* ownerWorld() {
        return owner_world_;
    }

    void addToWorld(btDynamicsWorld* p);
    void classBegin() override;
    void componentComplete() override;

    friend QDataStream& operator<<(QDataStream& s, const RigidBody& v);
    friend QDataStream& operator>>(QDataStream& s, RigidBody& v);

signals:
    void massChanged(float v);
    void scalingChanged(const Vector3& v);

protected:
    void preCollisionShapeChange() {
        if (owner_world_ != nullptr) {
            owner_world_->removeRigidBody(&rigid_body_);
        }
    }

    void postCollisionShapeChange() {
        if (owner_world_ != nullptr) {
            owner_world_->addRigidBody(&rigid_body_);
            // cleanProxyFromPairs?
        }
    }

    void setCollisionShape(btCollisionShape* p) {
        preCollisionShapeChange();
        rigid_body_.setCollisionShape(p);
        postCollisionShapeChange();
    }

private:
    class MotionState: public btMotionState {
    public:
        MotionState(Transformable& orig);

    public:
        void getWorldTransform(btTransform& world_transform) const override;
        void setWorldTransform(const btTransform& world_transform) override;

    private:
        Transformable& orig_;
    };

private:
    MotionState motion_state_;
    btRigidBody rigid_body_;
    btDynamicsWorld* owner_world_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_RIGIDBODY_H
