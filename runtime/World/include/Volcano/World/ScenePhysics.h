//
//
#ifndef VOLCANO_WORLD_PHYSICS_H
#define VOLCANO_WORLD_PHYSICS_H

#include <QObject>
#include <QVector3D>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Volcano/World/Common.h>
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

class Dynamic: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged FINAL)

public:
    Dynamic(QObject* parent = nullptr);

public:
    bool isEnabled() const {
        return enabled_;
	}

    void setEnabled(bool v) {
        if (enabled_ != v) {
            enabled_ = v;
            emit enabledChanged(v);
        }
    }

    void enable() {
        setEnabled(true);
    }

    void disable() {
        setEnabled(false);
    }

    const QVector3D& gravity() const {
		return gravity_;
	}

    JPH::BodyInterface& bodyInterface() noexcept {
        return physics_system_.GetBodyInterface();
    }

    const JPH::BodyInterface& bodyInterface() const noexcept {
        return physics_system_.GetBodyInterface();
    }

    void setGravity(const QVector3D& v) noexcept;
    void update(Clock::duration elapsed) noexcept;

signals:
    void enabledChanged(bool v);
    void gravityChanged(const QVector3D& v);

private:
    bool enabled_;
    QVector3D gravity_;
    JPH::PhysicsSystem physics_system_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_PHYSICS_H
