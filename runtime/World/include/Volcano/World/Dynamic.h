//
//
#ifndef VOLCANO_WORLD_DYNAMIC_H
#define VOLCANO_WORLD_DYNAMIC_H

#include <QObject>
#include <QVector3D>

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

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

    void setGravity(const QVector3D& v) {
        if (!qFuzzyCompare(gravity_, v)) {
            gravity_ = v;
            world_.setGravity(btVector3(v.x(), v.y(), v.x()));
            emit gravityChanged(v);
        }
	}

    void update(Duration elapsed) {
        if (enabled_) {
            world_.stepSimulation(durationToMicroseconds(elapsed) / 1000000.0);
        }
    }

signals:
    void enabledChanged(bool v);
    void gravityChanged(const QVector3D& v);

private:
    bool enabled_;
    QVector3D gravity_;
    btDefaultCollisionConfiguration collision_configuration_;
    btCollisionDispatcher  collision_dispatcher_;
    btDbvtBroadphase overlapping_pair_cache_;
    btSequentialImpulseConstraintSolver sequential_impulse_constraint_solver_;
    btDiscreteDynamicsWorld world_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_DYNAMIC_H
