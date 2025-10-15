//
//
#ifndef VOLCANO_FRAMEWORK_PHYSICS_H
#define VOLCANO_FRAMEWORK_PHYSICS_H

#include <chrono>

#include <btBulletDynamicsCommon.h>

#include <Volcano/World/Scene.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

class Physics {
public:
    using Clock = std::chrono::steady_clock;

public:
    Physics(World::Scene& scene);
    virtual ~Physics() = default;

public:
    bool isEnabled() const noexcept {
        return enabled_;
    }

    void enable() noexcept {
        enabled_ = true;
    }

    void disable() noexcept {
        enabled_ = false;
    }

    void frame(Clock::duration elapsed) noexcept {
        if (enabled_) {
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            bt_world_.stepSimulation(elapsed_ms);
        }
    }

private:
    World::Scene& scene_;
    bool enabled_;
    btDefaultCollisionConfiguration bt_collision_configuration_;
    btCollisionDispatcher bt_collision_dispatcher_;
    btDbvtBroadphase bt_broad_phase_;
    btSequentialImpulseConstraintSolver bt_solver_;
    btDiscreteDynamicsWorld bt_world_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_PHYSICS_H