//
//
#include <Volcano/Physics/BoxRigidBody.h>
#include <Volcano/Physics/CapsuleRigidBody.h>
#include <Volcano/Physics/ConeRigidBody.h>
#include <Volcano/Physics/CylinderRigidBody.h>
#include <Volcano/Physics/SphereRigidBody.h>
#include <Volcano/Physics/StaticPlaneRigidBody.h>
#include <Volcano/Physics/TriangleMeshRigidBody.h>
#include <Volcano/Physics/Simulator.h>

VOLCANO_PHYSICS_BEGIN

Simulator::Simulator(World::Scene& scene, QObject* parent)
    : World::Listener(scene, parent)
    , enabled_(true)
    , gravity_(0.0f, -9.8f, 0.0f)
    , bt_collision_dispatcher_(btCollisionDispatcher(&bt_collision_configuration_))
    , bt_dynamics_world_(
          &bt_collision_dispatcher_,
          &bt_overlapping_pair_cache_,
          &bt_sequential_impulse_constraint_solver_,
          &bt_collision_configuration_) {
    bt_dynamics_world_.setGravity(btVector3(gravity_.x(), gravity_.y(), gravity_.x()));
}

void Simulator::onComponentAdded(World::Entity* entity, World::Component* component) {
    auto rigid_body = qobject_cast<RigidBody*>(component);
    if (rigid_body != nullptr) {
        rigid_body->setOwnerWorld(&bt_dynamics_world_);
        rigid_body->attachWorldTransform(&entity->transform());
        bt_dynamics_world_.addRigidBody(rigid_body);
    }
}

void Simulator::onComponentRemoved(World::Entity* entity, World::Component* component) {
    auto rigid_body = qobject_cast<RigidBody*>(component);
    if (rigid_body != nullptr) {
        rigid_body->setOwnerWorld(nullptr);
        rigid_body->attachWorldTransform(nullptr);
        bt_dynamics_world_.removeRigidBody(rigid_body);
    }
}

VOLCANO_PHYSICS_END
