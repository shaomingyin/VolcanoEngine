//
//
#ifndef VOLCANO_WORLD_PHYSICS_H
#define VOLCANO_WORLD_PHYSICS_H

#include <memory>

#include <QObject>
#include <QVector3D>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/EPhysicsUpdateError.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class PhysicsPrivate;

class Physics: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)

public:
    explicit Physics(QObject* parent = nullptr);
    ~Physics() override;

public:
    Q_INVOKABLE void enable();
    Q_INVOKABLE void disable();
    bool isEnabled() const noexcept;
    void setEnabled(bool enabled);
    const QVector3D& gravity() const noexcept;
    void setGravity(const QVector3D& gravity);
    JPH::PhysicsSystem* physicsSystem() noexcept;
    const JPH::PhysicsSystem* physicsSystem() const noexcept;
    JPH::EPhysicsUpdateError update(Clock::duration deltaTime, int collisionSteps = 1);

signals:
    void enabledChanged(bool enabled);
    void gravityChanged(const QVector3D& gravity);

private:
    QVector3D gravity_ { 0.0f, -9.81f, 0.0f };
    std::unique_ptr<PhysicsPrivate> d_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_PHYSICS_H
