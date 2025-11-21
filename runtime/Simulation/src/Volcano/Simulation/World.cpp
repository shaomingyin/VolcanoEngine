//
//
#include <async++.h>
#include <edyn/edyn.hpp>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Simulation/World.hpp>

VOLCANO_SIMULATION_BEGIN

World::World(Context& context)
    : context_(context)
    , state_(State::Loading)
    , gravity_(0.0f, -9.8f, 0.0f) {
}

World::~World() {
    // TODO loading_task_
}

void World::load() {

}

void World::enablePhysics() noexcept {
    physics_enabled_ = true;
    for (auto& scene: scenes_) {
        edyn::set_paused(scene, false);
    }
}

void World::disablePhysics() noexcept {
    physics_enabled_ = false;
    for (auto& scene: scenes_) {
        edyn::set_paused(scene, false);
    }
}

void World::setGravity(const Eigen::Vector3f& v) noexcept {
    gravity_ = v;
    for (auto& scene: scenes_) {
        edyn::set_gravity(scene, { v.x(), v.y(), v.z() });
    }
}

void World::update(Clock::duration elapsed) {
    if (state_ == State::Ready && physics_enabled_) {
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        auto elapsed_ms_double = double(elapsed_ms) / 1000;
        for (auto& scene: scenes_) {
            edyn::update(scene, elapsed_ms_double);
        }
    }
}

VOLCANO_SIMULATION_END
