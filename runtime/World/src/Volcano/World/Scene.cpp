//
//
#include <Volcano/World/BoxRigidBody.h>
#include <Volcano/World/CapsuleRigidBody.h>
#include <Volcano/World/ConeRigidBody.h>
#include <Volcano/World/CylinderRigidBody.h>
#include <Volcano/World/SphereRigidBody.h>
#include <Volcano/World/StaticPlaneRigidBody.h>
#include <Volcano/World/TriangleMeshRigidBody.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene()
    : global_(registry_, registry_.create(), "<Global>", Eigen::Affine3f::Identity())
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
    registry_.on_construct<BoxRigidBody>().connect<&Scene::rigidBodyAdded<BoxRigidBody>>(this);
    registry_.on_construct<CapsuleRigidBody>().connect<&Scene::rigidBodyAdded<CapsuleRigidBody>>(this);
    registry_.on_construct<ConeRigidBody>().connect<&Scene::rigidBodyAdded<ConeRigidBody>>(this);
    registry_.on_construct<CylinderRigidBody>().connect<&Scene::rigidBodyAdded<CylinderRigidBody>>(this);
    registry_.on_construct<SphereRigidBody>().connect<&Scene::rigidBodyAdded<SphereRigidBody>>(this);
    registry_.on_construct<StaticPlaneRigidBody>().connect<&Scene::rigidBodyAdded<StaticPlaneRigidBody>>(this);
    registry_.on_construct<TriangleMeshRigidBody>().connect<&Scene::rigidBodyAdded<TriangleMeshRigidBody>>(this);

    registry_.on_destroy<BoxRigidBody>().connect<&Scene::rigidBodyRemoved<BoxRigidBody>>(this);
    registry_.on_destroy<CapsuleRigidBody>().connect<&Scene::rigidBodyRemoved<CapsuleRigidBody>>(this);
    registry_.on_destroy<ConeRigidBody>().connect<&Scene::rigidBodyRemoved<ConeRigidBody>>(this);
    registry_.on_destroy<CylinderRigidBody>().connect<&Scene::rigidBodyRemoved<CylinderRigidBody>>(this);
    registry_.on_destroy<SphereRigidBody>().connect<&Scene::rigidBodyRemoved<SphereRigidBody>>(this);
    registry_.on_destroy<StaticPlaneRigidBody>().connect<&Scene::rigidBodyRemoved<StaticPlaneRigidBody>>(this);
    registry_.on_destroy<TriangleMeshRigidBody>().connect<&Scene::rigidBodyRemoved<TriangleMeshRigidBody>>(this);

    bt_dynamics_world_->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

Scene::~Scene() {
    registry_.on_construct<BoxRigidBody>().disconnect<&Scene::rigidBodyAdded<BoxRigidBody>>(this);
    registry_.on_construct<CapsuleRigidBody>().disconnect<&Scene::rigidBodyAdded<CapsuleRigidBody>>(this);
    registry_.on_construct<ConeRigidBody>().disconnect<&Scene::rigidBodyAdded<ConeRigidBody>>(this);
    registry_.on_construct<CylinderRigidBody>().disconnect<&Scene::rigidBodyAdded<CylinderRigidBody>>(this);
    registry_.on_construct<SphereRigidBody>().disconnect<&Scene::rigidBodyAdded<SphereRigidBody>>(this);
    registry_.on_construct<StaticPlaneRigidBody>().disconnect<&Scene::rigidBodyAdded<StaticPlaneRigidBody>>(this);
    registry_.on_construct<TriangleMeshRigidBody>().disconnect<&Scene::rigidBodyAdded<TriangleMeshRigidBody>>(this);

    registry_.on_destroy<BoxRigidBody>().disconnect<&Scene::rigidBodyRemoved<BoxRigidBody>>(this);
    registry_.on_destroy<CapsuleRigidBody>().disconnect<&Scene::rigidBodyRemoved<CapsuleRigidBody>>(this);
    registry_.on_destroy<ConeRigidBody>().disconnect<&Scene::rigidBodyRemoved<ConeRigidBody>>(this);
    registry_.on_destroy<CylinderRigidBody>().disconnect<&Scene::rigidBodyRemoved<CylinderRigidBody>>(this);
    registry_.on_destroy<SphereRigidBody>().disconnect<&Scene::rigidBodyRemoved<SphereRigidBody>>(this);
    registry_.on_destroy<StaticPlaneRigidBody>().disconnect<&Scene::rigidBodyRemoved<StaticPlaneRigidBody>>(this);
    registry_.on_destroy<TriangleMeshRigidBody>().disconnect<&Scene::rigidBodyRemoved<TriangleMeshRigidBody>>(this);

    global_.destroy();
}

void Scene::frame(Duration elapsed) {
    if (physics_enabled_) {
        bt_dynamics_world_->stepSimulation(durationToMicroseconds(elapsed) / 1000000.0);
    }
}

VOLCANO_WORLD_END
