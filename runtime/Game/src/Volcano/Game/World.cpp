//
//
#include <Volcano/Game/Basic.h>
#include <Volcano/Game/BoxRigidBody.h>
#include <Volcano/Game/CapsuleRigidBody.h>
#include <Volcano/Game/ConeRigidBody.h>
#include <Volcano/Game/CylinderRigidBody.h>
#include <Volcano/Game/SphereRigidBody.h>
#include <Volcano/Game/StaticPlaneRigidBody.h>
#include <Volcano/Game/TriangleMeshRigidBody.h>
#include <Volcano/Game/World.h>

VOLCANO_GAME_BEGIN

World::World()
    : global_(registry_.create())
    , physics_enabled_(true)
    , bt_collision_configuration_(new btDefaultCollisionConfiguration())
    , bt_collision_dispatcher_(new btCollisionDispatcher(bt_collision_configuration_.get()))
    , bt_overlapping_pair_cache_(new btDbvtBroadphase())
    , bt_sequential_impulse_constraint_solver_(new btSequentialImpulseConstraintSolver)
    , bt_dynamics_world_(new btDiscreteDynamicsWorld(
        bt_collision_dispatcher_.get(),
        bt_overlapping_pair_cache_.get(),
        bt_sequential_impulse_constraint_solver_.get(),
        bt_collision_configuration_.get())) {
    registry_.on_construct<BoxRigidBody>().connect<&World::boxRigidBodyAdded>(this);
    registry_.on_construct<CapsuleRigidBody>().connect<&World::capsuleRigidBodyAdded>(this);
    registry_.on_construct<ConeRigidBody>().connect<&World::coneRigidBodyAdded>(this);
    registry_.on_construct<CylinderRigidBody>().connect<&World::cylinderRigidBodyAdded>(this);
    registry_.on_construct<SphereRigidBody>().connect<&World::sphereRigidBodyAdded>(this);
    registry_.on_construct<StaticPlaneRigidBody>().connect<&World::staticPlaneRigidBodyAdded>(this);
    registry_.on_construct<TriangleMeshRigidBody>().connect<&World::triangleMeshRigidBodyAdded>(this);

    registry_.on_destroy<BoxRigidBody>().connect<&World::boxRigidBodyRemoved>(this);
    registry_.on_destroy<CapsuleRigidBody>().connect<&World::capsuleRigidBodyRemoved>(this);
    registry_.on_destroy<ConeRigidBody>().connect<&World::coneRigidBodyRemoved>(this);
    registry_.on_destroy<CylinderRigidBody>().connect<&World::cylinderRigidBodyRemoved>(this);
    registry_.on_destroy<SphereRigidBody>().connect<&World::sphereRigidBodyRemoved>(this);
    registry_.on_destroy<StaticPlaneRigidBody>().connect<&World::staticPlaneRigidBodyRemoved>(this);
    registry_.on_destroy<TriangleMeshRigidBody>().connect<&World::triangleMeshRigidBodyRemoved>(this);
}

World::~World() {
    registry_.on_construct<BoxRigidBody>().disconnect<&World::boxRigidBodyAdded>(this);
    registry_.on_construct<CapsuleRigidBody>().disconnect<&World::capsuleRigidBodyAdded>(this);
    registry_.on_construct<ConeRigidBody>().disconnect<&World::coneRigidBodyAdded>(this);
    registry_.on_construct<CylinderRigidBody>().disconnect<&World::cylinderRigidBodyAdded>(this);
    registry_.on_construct<SphereRigidBody>().disconnect<&World::sphereRigidBodyAdded>(this);
    registry_.on_construct<StaticPlaneRigidBody>().disconnect<&World::staticPlaneRigidBodyAdded>(this);
    registry_.on_construct<TriangleMeshRigidBody>().disconnect<&World::triangleMeshRigidBodyAdded>(this);

    registry_.on_destroy<BoxRigidBody>().disconnect<&World::boxRigidBodyRemoved>(this);
    registry_.on_destroy<CapsuleRigidBody>().disconnect<&World::capsuleRigidBodyRemoved>(this);
    registry_.on_destroy<ConeRigidBody>().disconnect<&World::coneRigidBodyRemoved>(this);
    registry_.on_destroy<CylinderRigidBody>().disconnect<&World::cylinderRigidBodyRemoved>(this);
    registry_.on_destroy<SphereRigidBody>().disconnect<&World::sphereRigidBodyRemoved>(this);
    registry_.on_destroy<StaticPlaneRigidBody>().disconnect<&World::staticPlaneRigidBodyRemoved>(this);
    registry_.on_destroy<TriangleMeshRigidBody>().disconnect<&World::triangleMeshRigidBodyRemoved>(this);

    registry_.destroy(global_);
}

void World::update(Duration elapsed) {
    if (physics_enabled_) {
        bt_dynamics_world_->stepSimulation(durationToMicroseconds(elapsed) / 1000000.0);
    }
}

void World::boxRigidBodyAdded(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& basic = registry.get<Basic>(entity);
    auto& rigid_body = registry.get<BoxRigidBody>(entity);
    rigid_body.setMotionState(&basic);
    bt_dynamics_world_->addRigidBody(&rigid_body);
}

void World::boxRigidBodyRemoved(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& rigid_body = registry.get<BoxRigidBody>(entity);
    rigid_body.setMotionState(nullptr);
    bt_dynamics_world_->removeRigidBody(&rigid_body);
}

void World::capsuleRigidBodyAdded(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& basic = registry.get<Basic>(entity);
    auto& rigid_body = registry.get<CapsuleRigidBody>(entity);
    rigid_body.setMotionState(&basic);
    bt_dynamics_world_->addRigidBody(&rigid_body);
}

void World::capsuleRigidBodyRemoved(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& rigid_body = registry.get<CapsuleRigidBody>(entity);
    rigid_body.setMotionState(nullptr);
    bt_dynamics_world_->removeRigidBody(&rigid_body);
}

void World::coneRigidBodyAdded(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& basic = registry.get<Basic>(entity);
    auto& rigid_body = registry.get<ConeRigidBody>(entity);
    rigid_body.setMotionState(&basic);
    bt_dynamics_world_->addRigidBody(&rigid_body);
}

void World::coneRigidBodyRemoved(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& rigid_body = registry.get<ConeRigidBody>(entity);
    rigid_body.setMotionState(nullptr);
    bt_dynamics_world_->removeRigidBody(&rigid_body);
}

void World::cylinderRigidBodyAdded(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& basic = registry.get<Basic>(entity);
    auto& rigid_body = registry.get<CylinderRigidBody>(entity);
    rigid_body.setMotionState(&basic);
    bt_dynamics_world_->addRigidBody(&rigid_body);
}

void World::cylinderRigidBodyRemoved(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& rigid_body = registry.get<CylinderRigidBody>(entity);
    rigid_body.setMotionState(nullptr);
    bt_dynamics_world_->removeRigidBody(&rigid_body);
}

void World::sphereRigidBodyAdded(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& basic = registry.get<Basic>(entity);
    auto& rigid_body = registry.get<SphereRigidBody>(entity);
    rigid_body.setMotionState(&basic);
    bt_dynamics_world_->addRigidBody(&rigid_body);
}

void World::sphereRigidBodyRemoved(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& rigid_body = registry.get<SphereRigidBody>(entity);
    rigid_body.setMotionState(nullptr);
    bt_dynamics_world_->removeRigidBody(&rigid_body);
}

void World::staticPlaneRigidBodyAdded(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& basic = registry.get<Basic>(entity);
    auto& rigid_body = registry.get<StaticPlaneRigidBody>(entity);
    rigid_body.setMotionState(&basic);
    bt_dynamics_world_->addRigidBody(&rigid_body);
}

void World::staticPlaneRigidBodyRemoved(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& rigid_body = registry.get<StaticPlaneRigidBody>(entity);
    rigid_body.setMotionState(nullptr);
    bt_dynamics_world_->removeRigidBody(&rigid_body);
}

void World::triangleMeshRigidBodyAdded(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& basic = registry.get<Basic>(entity);
    auto& rigid_body = registry.get<TriangleMeshRigidBody>(entity);
    rigid_body.setMotionState(&basic);
    bt_dynamics_world_->addRigidBody(&rigid_body);
}

void World::triangleMeshRigidBodyRemoved(entt::registry& registry, entt::entity entity) {
    VOLCANO_ASSERT(&registry_ == &registry);
    auto& rigid_body = registry.get<TriangleMeshRigidBody>(entity);
    rigid_body.setMotionState(nullptr);
    bt_dynamics_world_->removeRigidBody(&rigid_body);
}

VOLCANO_GAME_END
