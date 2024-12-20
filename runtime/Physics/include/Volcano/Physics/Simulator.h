//
//
#ifndef VOLCANO_PHYSICS_SIMULATOR_H
#define VOLCANO_PHYSICS_SIMULATOR_H

#include <QObject>
#include <QVector3D>

#include <Volcano/World/Scene.h>
#include <Volcano/World/Component.h>
#include <Volcano/World/Listener.h>
#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class Simulator: public World::Listener {
    Q_OBJECT

public:
    Simulator(World::Scene& scene, QObject* parent = nullptr);
    ~Simulator() override = default;

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
		gravity_ = v;
        bt_dynamics_world_.setGravity(btVector3(v.x(), v.y(), v.x()));
	}

    void update(Duration elapsed) {
        if (enabled_) {
            bt_dynamics_world_.stepSimulation(durationToMicroseconds(elapsed) / 1000000.0);
        }
    }

signals:
    void enabledChanged(bool v);

private slots:
    void onComponentAdded(World::Entity* entity, World::Component* component) override;
    void onComponentRemoved(World::Entity* entity, World::Component* component) override;

private:
    bool enabled_;
    QVector3D gravity_;
    btDefaultCollisionConfiguration bt_collision_configuration_;
    btCollisionDispatcher  bt_collision_dispatcher_;
    btDbvtBroadphase bt_overlapping_pair_cache_;
    btSequentialImpulseConstraintSolver bt_sequential_impulse_constraint_solver_;
    btDiscreteDynamicsWorld bt_dynamics_world_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_SIMULATOR_H
