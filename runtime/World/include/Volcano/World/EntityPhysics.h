//
//
#ifndef VOLCANO_WORLD_ENTITYPHYSICS_H
#define VOLCANO_WORLD_ENTITYPHYSICS_H

#include <QVector3D>
#include <QObject>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include <Volcano/World/Common.h>
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

class EntityPhysics: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(CollisionShape* collisonShape READ collisonShape WRITE setCollisonShape NOTIFY collisonShapeChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged FINAL)

public:
    EntityPhysics(QObject* parent = nullptr);
    ~EntityPhysics() override;

public:
    void setBodyInterface(JPH::BodyInterface* body_interface) noexcept;

    bool isEnabled() const noexcept {
        return enabled_;
    }

    void setEnabled(bool v) noexcept;

    CollisionShape* collisionShape() const noexcept {
        return collision_shape_;
    }

    void setCollisionShape(CollisionShape* p) noexcept;

signals:
    void enabledChanged(bool v);
    void collisionShapeChanged(CollisionShape* p);

private:
    bool enabled_;
    JPH::BodyInterface* body_interface_;
    JPH::BodyID body_id_;
    CollisionShape* collision_shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENTITYPHYSICS_H
