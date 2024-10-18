//
//
#include <Volcano/Game/RigidBody.h>
#include <Volcano/Game/World.h>

VOLCANO_GAME_BEGIN

World::World()
    : builtin_(registry_.create())
    , gravity_(0.0f, -9.8f, 0.0f) {
    registry_.on_construct<RigidBody>().connect<&World::rigidBodyCreated>(this);
    registry_.on_destroy<RigidBody>().connect<&World::rigidBodyDestroyed>(this);
}

World::~World() {
    registry_.on_destroy<RigidBody>().disconnect<&World::rigidBodyDestroyed>(this);
    registry_.on_construct<RigidBody>().disconnect<&World::rigidBodyCreated>(this);
    registry_.destroy(builtin_);
}

void World::update(Duration elapsed) {
    if (bt_dynamics_world_) {
        bt_dynamics_world_->stepSimulation(durationToMicroseconds(elapsed) / 1000000.0);
    }
}

void World::enablePhysics() {
    if (bt_dynamics_world_) {
        return;
    }
}

void World::disablePhysics() {
    if (!bt_dynamics_world_) {
        return;
    }
}

void World::setGravity(Eigen::Vector3f v) {
    gravity_ = v;
    if (bt_dynamics_world_) {
        bt_dynamics_world_->setGravity(btVector3(v.x(), v.y(), v.z()));
    }
}

void World::rigidBodyCreated(entt::registry& reg, entt::entity ent) {
    VOLCANO_ASSERT(&registry_ == &reg);
    auto& basic = reg.get<Basic>(ent);
    auto& rigid_body = reg.get<RigidBody>(ent);
    // TODO bullet.
}

void World::rigidBodyDestroyed(entt::registry& reg, entt::entity ent) {

}

VOLCANO_GAME_END
