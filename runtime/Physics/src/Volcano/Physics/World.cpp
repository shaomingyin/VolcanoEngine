//
//
#include <Volcano/Physics/BoxRigidBody.h>
#include <Volcano/Physics/CapsuleRigidBody.h>
#include <Volcano/Physics/ConeRigidBody.h>
#include <Volcano/Physics/CylinderRigidBody.h>
#include <Volcano/Physics/SphereRigidBody.h>
#include <Volcano/Physics/StaticPlaneRigidBody.h>
#include <Volcano/Physics/TriangleMeshRigidBody.h>
#include <Volcano/Physics/World.h>

VOLCANO_PHYSICS_BEGIN

World::World(entt::registry& registry)
    : registry_(registry)
    , gravity_(0.0f, -9.8f, 0.0f) {
    registry_.on_construct<BoxRigidBody>().connect<&World::onRigidBodyAdded<BoxRigidBody>>(this);
    registry_.on_construct<CapsuleRigidBody>().connect<&World::onRigidBodyAdded<CapsuleRigidBody>>(this);
    registry_.on_construct<ConeRigidBody>().connect<&World::onRigidBodyAdded<ConeRigidBody>>(this);
    registry_.on_construct<CylinderRigidBody>().connect<&World::onRigidBodyAdded<CylinderRigidBody>>(this);
    registry_.on_construct<SphereRigidBody>().connect<&World::onRigidBodyAdded<SphereRigidBody>>(this);
    registry_.on_construct<StaticPlaneRigidBody>().connect<&World::onRigidBodyAdded<StaticPlaneRigidBody>>(this);
    registry_.on_construct<TriangleMeshRigidBody>().connect<&World::onRigidBodyAdded<TriangleMeshRigidBody>>(this);

    registry_.on_destroy<BoxRigidBody>().connect<&World::onRigidBodyRemoved<BoxRigidBody>>(this);
    registry_.on_destroy<CapsuleRigidBody>().connect<&World::onRigidBodyRemoved<CapsuleRigidBody>>(this);
    registry_.on_destroy<ConeRigidBody>().connect<&World::onRigidBodyRemoved<ConeRigidBody>>(this);
    registry_.on_destroy<CylinderRigidBody>().connect<&World::onRigidBodyRemoved<CylinderRigidBody>>(this);
    registry_.on_destroy<SphereRigidBody>().connect<&World::onRigidBodyRemoved<SphereRigidBody>>(this);
    registry_.on_destroy<StaticPlaneRigidBody>().connect<&World::onRigidBodyRemoved<StaticPlaneRigidBody>>(this);
    registry_.on_destroy<TriangleMeshRigidBody>().connect<&World::onRigidBodyRemoved<TriangleMeshRigidBody>>(this);

    bt_dynamics_world_->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

World::~World() {
    registry_.on_construct<BoxRigidBody>().disconnect<&World::onRigidBodyAdded<BoxRigidBody>>(this);
    registry_.on_construct<CapsuleRigidBody>().disconnect<&World::onRigidBodyAdded<CapsuleRigidBody>>(this);
    registry_.on_construct<ConeRigidBody>().disconnect<&World::onRigidBodyAdded<ConeRigidBody>>(this);
    registry_.on_construct<CylinderRigidBody>().disconnect<&World::onRigidBodyAdded<CylinderRigidBody>>(this);
    registry_.on_construct<SphereRigidBody>().disconnect<&World::onRigidBodyAdded<SphereRigidBody>>(this);
    registry_.on_construct<StaticPlaneRigidBody>().disconnect<&World::onRigidBodyAdded<StaticPlaneRigidBody>>(this);
    registry_.on_construct<TriangleMeshRigidBody>().disconnect<&World::onRigidBodyAdded<TriangleMeshRigidBody>>(this);

    registry_.on_destroy<BoxRigidBody>().disconnect<&World::onRigidBodyRemoved<BoxRigidBody>>(this);
    registry_.on_destroy<CapsuleRigidBody>().disconnect<&World::onRigidBodyRemoved<CapsuleRigidBody>>(this);
    registry_.on_destroy<ConeRigidBody>().disconnect<&World::onRigidBodyRemoved<ConeRigidBody>>(this);
    registry_.on_destroy<CylinderRigidBody>().disconnect<&World::onRigidBodyRemoved<CylinderRigidBody>>(this);
    registry_.on_destroy<SphereRigidBody>().disconnect<&World::onRigidBodyRemoved<SphereRigidBody>>(this);
    registry_.on_destroy<StaticPlaneRigidBody>().disconnect<&World::onRigidBodyRemoved<StaticPlaneRigidBody>>(this);
    registry_.on_destroy<TriangleMeshRigidBody>().disconnect<&World::onRigidBodyRemoved<TriangleMeshRigidBody>>(this);
}

void World::enable() {
    if (bt_dynamics_world_) {
        return;
    }

    bt_collision_configuration_ = std::make_unique<btDefaultCollisionConfiguration>();
    bt_collision_dispatcher_ = std::make_unique<btCollisionDispatcher>(bt_collision_configuration_.get());
    bt_overlapping_pair_cache_ = std::make_unique<btDbvtBroadphase>();
    bt_sequential_impulse_constraint_solver_ = std::make_unique<btSequentialImpulseConstraintSolver>();

    bt_dynamics_world_ = std::make_unique<btDiscreteDynamicsWorld>(
        bt_collision_dispatcher_.get(),
        bt_overlapping_pair_cache_.get(),
        bt_sequential_impulse_constraint_solver_.get(),
        bt_collision_configuration_.get());

    registry_.view<Volcano::World::Inherency, BoxRigidBody>().each(
        [this](auto& inherency, auto& body) {
            addRigidBody(body, &inherency);
        });

    registry_.view<Volcano::World::Inherency, CapsuleRigidBody>().each(
        [this](auto& inherency, auto& body) {
            addRigidBody(body, &inherency);
        });

    registry_.view<Volcano::World::Inherency, ConeRigidBody>().each(
        [this](auto& inherency, auto& body) {
            addRigidBody(body, &inherency);
        });

    registry_.view<Volcano::World::Inherency, CylinderRigidBody>().each(
        [this](auto& inherency, auto& body) {
            addRigidBody(body, &inherency);
        });
    registry_.view<Volcano::World::Inherency, SphereRigidBody>().each(
        [this](auto& inherency, auto& body) {
            addRigidBody(body, &inherency);
        });

    registry_.view<Volcano::World::Inherency, StaticPlaneRigidBody>().each(
        [this](auto& inherency, auto& body) {
            addRigidBody(body, &inherency);
        });

    registry_.view<Volcano::World::Inherency, TriangleMeshRigidBody>().each(
        [this](auto& inherency, auto& body) {
            addRigidBody(body, &inherency);
        });
}

void World::disable() {
    bt_dynamics_world_.reset();
    bt_sequential_impulse_constraint_solver_.reset();
    bt_overlapping_pair_cache_.reset();
    bt_collision_dispatcher_.reset();
    bt_collision_configuration_.reset();
}

void World::addRigidBody(RigidBody& body, Volcano::World::Inherency* inherency) {
    if (inherency != nullptr) {
        body.attachWorldTransform(inherency->transform());
        bt_dynamics_world_->addRigidBody(&body);
    } else {
        bt_dynamics_world_->addRigidBody(&body);
    }
}

VOLCANO_PHYSICS_END
