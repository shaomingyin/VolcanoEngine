//
//
#ifndef VOLCANO_GAME_RIGIDBODY_H
#define VOLCANO_GAME_RIGIDBODY_H

#include <QVector3D>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Transformable.h>

VOLCANO_GAME_BEGIN

class RigidBody: public Transformable {
    Q_OBJECT
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged FINAL)

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

    QVector3D scale() const {
        const btVector3& r = rigid_body_.getCollisionShape()->getLocalScaling();
        return { r.x(), r.y(), r.z() };
    }

    void setScale(const QVector3D& v) {
        rigid_body_.getCollisionShape()->setLocalScaling({ v.x(), v.y(), v.z() });
        emit scaleChanged(v);
    }

    btDynamicsWorld* ownerWorld() {
        return owner_world_;
    }

    void addToWorld(btDynamicsWorld* p);
    void classBegin() override;
    void componentComplete() override;

signals:
    void massChanged(float v);
    void scaleChanged(const QVector3D& v);

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

VOLCANO_GAME_END

#endif // VOLCANO_GAME_RIGIDBODY_H
