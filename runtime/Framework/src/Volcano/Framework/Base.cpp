//
//
#include <thread>

#include <Volcano/Framework/Base.h>

VOLCANO_FRAMEWORK_BEGIN

using namespace std::chrono_literals;

Base::Base(const std::string& name)
    : name_(name)
    , running_(false)
    , frame_last_(Clock::now())
    , frame_count_last_(frame_last_)
    , frame_count_(0)
    , frame_count_per_second_(0)
    , bt_enabled_(true)
    , bt_collision_dispatcher_(&bt_collision_configuration_)
    , bt_world_(&bt_collision_dispatcher_, &bt_broad_phase_, &bt_solver_, &bt_collision_configuration_) {
    setFpsMax(60);
    auto& world_scene = scene();
    // world_scene.on_construct<RigidBocy>().
}

void Base::run() {
    running_ = true;
    while (running_) {
        auto now = Clock::now();
        if ((now - frame_count_last_) >= 1s) {
            frame_count_per_second_ = frame_count_;
            frame_count_ = 0;
            frame_count_last_ = now;
        }
        scheduler_.run_all_tasks();
        frame(now - frame_last_);
        auto update_elapsed = Clock::now() - now;
        if (update_elapsed < min_elapsed_) {
            std::this_thread::sleep_for(min_elapsed_ - update_elapsed);
        }
    }
}

void Base::quit() noexcept {
    running_ = false;
}

const std::string& Base::name() const noexcept {
    return name_;
}

unsigned long Base::fps() const noexcept {
    return frame_count_per_second_;
}

unsigned long Base::fpsMax() const noexcept {
    return (1000000000ns / min_elapsed_);
}

void Base::setFpsMax(unsigned long v) noexcept {
    if (v < 1) {
        v = 1;
    }
    min_elapsed_ = (1000000000ns / v);
}

bool Base::isPhysicsEnabled() const noexcept {
    return bt_enabled_;
}

void Base::enablePhysics() noexcept {
    bt_enabled_ = true;
}

void Base::disablePhysics() noexcept {
    bt_enabled_ = false;
}

void Base::frame(Clock::duration elapsed) {
    if (bt_enabled_) {
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        bt_world_.stepSimulation(elapsed_ms);
    }
}

VOLCANO_FRAMEWORK_END
