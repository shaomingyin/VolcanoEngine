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
    registry_.on_construct<btRigidBody>().connect<&World::rigidBodyAdded<btRigidBody>>(this);
    registry_.on_construct<BoxRigidBody>().connect<&World::rigidBodyAdded<BoxRigidBody>>(this);
    registry_.on_construct<CapsuleRigidBody>().connect<&World::rigidBodyAdded<CapsuleRigidBody>>(this);
    registry_.on_construct<ConeRigidBody>().connect<&World::rigidBodyAdded<ConeRigidBody>>(this);
    registry_.on_construct<CylinderRigidBody>().connect<&World::rigidBodyAdded<CylinderRigidBody>>(this);
    registry_.on_construct<SphereRigidBody>().connect<&World::rigidBodyAdded<SphereRigidBody>>(this);
    registry_.on_construct<StaticPlaneRigidBody>().connect<&World::rigidBodyAdded<StaticPlaneRigidBody>>(this);
    registry_.on_construct<TriangleMeshRigidBody>().connect<&World::rigidBodyAdded<TriangleMeshRigidBody>>(this);

    registry_.on_destroy<btRigidBody>().connect<&World::rigidBodyRemoved<btRigidBody>>(this);
    registry_.on_destroy<BoxRigidBody>().connect<&World::rigidBodyRemoved<BoxRigidBody>>(this);
    registry_.on_destroy<CapsuleRigidBody>().connect<&World::rigidBodyRemoved<CapsuleRigidBody>>(this);
    registry_.on_destroy<ConeRigidBody>().connect<&World::rigidBodyRemoved<ConeRigidBody>>(this);
    registry_.on_destroy<CylinderRigidBody>().connect<&World::rigidBodyRemoved<CylinderRigidBody>>(this);
    registry_.on_destroy<SphereRigidBody>().connect<&World::rigidBodyRemoved<SphereRigidBody>>(this);
    registry_.on_destroy<StaticPlaneRigidBody>().connect<&World::rigidBodyRemoved<StaticPlaneRigidBody>>(this);
    registry_.on_destroy<TriangleMeshRigidBody>().connect<&World::rigidBodyRemoved<TriangleMeshRigidBody>>(this);

    bt_dynamics_world_->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

World::~World() {
    registry_.on_construct<btRigidBody>().disconnect<&World::rigidBodyAdded<btRigidBody>>(this);
    registry_.on_construct<BoxRigidBody>().disconnect<&World::rigidBodyAdded<BoxRigidBody>>(this);
    registry_.on_construct<CapsuleRigidBody>().disconnect<&World::rigidBodyAdded<CapsuleRigidBody>>(this);
    registry_.on_construct<ConeRigidBody>().disconnect<&World::rigidBodyAdded<ConeRigidBody>>(this);
    registry_.on_construct<CylinderRigidBody>().disconnect<&World::rigidBodyAdded<CylinderRigidBody>>(this);
    registry_.on_construct<SphereRigidBody>().disconnect<&World::rigidBodyAdded<SphereRigidBody>>(this);
    registry_.on_construct<StaticPlaneRigidBody>().disconnect<&World::rigidBodyAdded<StaticPlaneRigidBody>>(this);
    registry_.on_construct<TriangleMeshRigidBody>().disconnect<&World::rigidBodyAdded<TriangleMeshRigidBody>>(this);

    registry_.on_destroy<btRigidBody>().disconnect<&World::rigidBodyRemoved<btRigidBody>>(this);
    registry_.on_destroy<BoxRigidBody>().disconnect<&World::rigidBodyRemoved<BoxRigidBody>>(this);
    registry_.on_destroy<CapsuleRigidBody>().disconnect<&World::rigidBodyRemoved<CapsuleRigidBody>>(this);
    registry_.on_destroy<ConeRigidBody>().disconnect<&World::rigidBodyRemoved<ConeRigidBody>>(this);
    registry_.on_destroy<CylinderRigidBody>().disconnect<&World::rigidBodyRemoved<CylinderRigidBody>>(this);
    registry_.on_destroy<SphereRigidBody>().disconnect<&World::rigidBodyRemoved<SphereRigidBody>>(this);
    registry_.on_destroy<StaticPlaneRigidBody>().disconnect<&World::rigidBodyRemoved<StaticPlaneRigidBody>>(this);
    registry_.on_destroy<TriangleMeshRigidBody>().disconnect<&World::rigidBodyRemoved<TriangleMeshRigidBody>>(this);

    registry_.destroy(global_);
}

void World::frame(Duration elapsed) {
    if (physics_enabled_) {
        bt_dynamics_world_->stepSimulation(durationToMicroseconds(elapsed) / 1000000.0);
    }
}

VOLCANO_GAME_END
