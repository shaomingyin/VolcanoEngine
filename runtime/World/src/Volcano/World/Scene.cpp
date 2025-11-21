//
//
#include <async++.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/World/Scene.hpp>

VOLCANO_WORLD_BEGIN

Scene::Scene(Context& context)
    : context_(context)
    , state_(State::Loading)
    , bt_dispatcher_(&bt_config_)
    , bt_world_(&bt_dispatcher_, &bt_broadphase_, &bt_solver_, &bt_config_)
    , global_(context)
    , avatar_(context) {
    bt_world_.setGravity({ 0.0f, -9.8f, 0.0f });
}

Scene::~Scene() {
    // TODO loading_task_
}

void Scene::load() {
}

void Scene::setGravity(const Eigen::Vector3f& v) noexcept {
    gravity_ = v;
    bt_world_.setGravity({ v.x(), v.y(), v.z() });
}

void Scene::stepSimulation(Clock::duration elapsed) {
    if (state_ == State::Ready && physics_enabled_) {
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        auto elapsed_ms_double = double(elapsed_ms) / 1000;
        bt_world_.stepSimulation(elapsed_ms_double);
    }
}

VOLCANO_WORLD_END
