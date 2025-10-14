//
//
#ifndef VOLCANO_FRAMEWORK_BASE_H
#define VOLCANO_FRAMEWORK_BASE_H

#include <string>

#include <btBulletDynamicsCommon.h>

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

class Base: public Context {
public:
    Base(const std::string& name);
    ~Base() override = default;

public:
    void run();
    void quit() noexcept override;
    const std::string& name() const noexcept override;
    unsigned long fps() const noexcept override;
    unsigned long fpsMax() const noexcept override;
    void setFpsMax(unsigned long v) noexcept override;
    bool isPhysicsEnabled() const noexcept override;
    void enablePhysics() noexcept override;
    void disablePhysics() noexcept override;

protected:
    virtual void frame(Clock::duration elapsed);

private:
    std::string name_;
    bool running_;
    async::fifo_scheduler scheduler_;
    Clock::duration min_elapsed_;
    Clock::time_point frame_last_;
    Clock::time_point frame_count_last_;
    unsigned long frame_count_;
    unsigned long frame_count_per_second_;
    World::Scene scene_;
    bool bt_enabled_;
    btDefaultCollisionConfiguration bt_collision_configuration_;
    btCollisionDispatcher bt_collision_dispatcher_;
    btDbvtBroadphase bt_broad_phase_;
    btSequentialImpulseConstraintSolver bt_solver_;
    btDiscreteDynamicsWorld bt_world_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_BASE_H
