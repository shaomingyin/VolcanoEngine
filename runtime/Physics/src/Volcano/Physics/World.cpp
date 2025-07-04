//
//
#include <Volcano/Physics/World.h>

VOLCANO_PHYSICS_BEGIN

World::World()
    : gravity_(0.0f, -9.8f, 0.0f)
	, bt_collsion_dispatcher_(&bt_collision_configuration_)
	, bt_dynamic_world_(&bt_collsion_dispatcher_, &bt_broadphase_interface_, &bt_constraint_solver_, &bt_collision_configuration_) {
    bt_dynamic_world_.setGravity(btVector3(gravity_.x(), gravity_.y(), gravity_.z()));
}

void World::setGravity(const Eigen::Vector3f& v) {
    gravity_ = v;
    bt_dynamic_world_.setGravity(btVector3(gravity_.x(), gravity_.y(), gravity_.z()));
}

void World::update(Duration elapsed) {
    if (!enabled_) {
        return;
    }

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    bt_dynamic_world_.stepSimulation(elapsed_ms / 1000.0);
}

VOLCANO_PHYSICS_END
