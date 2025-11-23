//
//
#include <async++.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(Context& context)
    : context_(context)
    , state_(State::Loading)
    , bt_dispatcher_(&bt_config_)
    , bt_world_(&bt_dispatcher_, &bt_broadphase_, &bt_solver_, &bt_config_)
    , global_(context)
    , avatar_(context) {
    bt_world_.setGravity({ 0.0f, -9.8f, 0.0f });
}

World::~World() {
    // TODO loading_task_
}

void World::load() {
}

void World::setGravity(const Eigen::Vector3f& v) noexcept {
    gravity_ = v;
    bt_world_.setGravity({ v.x(), v.y(), v.z() });
}

void World::stepSimulation(Clock::duration elapsed) {
    if (state_ == State::Ready && physics_enabled_) {
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        auto elapsed_ms_double = double(elapsed_ms) / 1000;
        bt_world_.stepSimulation(elapsed_ms_double);
    }
}

VOLCANO_GAME_END
