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
    : global_(*this, create())
    , gravity_(0.0f, -9.8f, 0.0f) {
    on_construct<BoxRigidBody>().connect<&Scene::onRigidBodyAdded<BoxRigidBody>>(this);
    on_construct<CapsuleRigidBody>().connect<&Scene::onRigidBodyAdded<CapsuleRigidBody>>(this);
    on_construct<ConeRigidBody>().connect<&Scene::onRigidBodyAdded<ConeRigidBody>>(this);
    on_construct<CylinderRigidBody>().connect<&Scene::onRigidBodyAdded<CylinderRigidBody>>(this);
    on_construct<SphereRigidBody>().connect<&Scene::onRigidBodyAdded<SphereRigidBody>>(this);
    on_construct<StaticPlaneRigidBody>().connect<&Scene::onRigidBodyAdded<StaticPlaneRigidBody>>(this);
    on_construct<TriangleMeshRigidBody>().connect<&Scene::onRigidBodyAdded<TriangleMeshRigidBody>>(this);

    on_destroy<BoxRigidBody>().connect<&Scene::onRigidBodyRemoved<BoxRigidBody>>(this);
    on_destroy<CapsuleRigidBody>().connect<&Scene::onRigidBodyRemoved<CapsuleRigidBody>>(this);
    on_destroy<ConeRigidBody>().connect<&Scene::onRigidBodyRemoved<ConeRigidBody>>(this);
    on_destroy<CylinderRigidBody>().connect<&Scene::onRigidBodyRemoved<CylinderRigidBody>>(this);
    on_destroy<SphereRigidBody>().connect<&Scene::onRigidBodyRemoved<SphereRigidBody>>(this);
    on_destroy<StaticPlaneRigidBody>().connect<&Scene::onRigidBodyRemoved<StaticPlaneRigidBody>>(this);
    on_destroy<TriangleMeshRigidBody>().connect<&Scene::onRigidBodyRemoved<TriangleMeshRigidBody>>(this);
}

Scene::~Scene() {
    on_construct<BoxRigidBody>().disconnect<&Scene::onRigidBodyAdded<BoxRigidBody>>(this);
    on_construct<CapsuleRigidBody>().disconnect<&Scene::onRigidBodyAdded<CapsuleRigidBody>>(this);
    on_construct<ConeRigidBody>().disconnect<&Scene::onRigidBodyAdded<ConeRigidBody>>(this);
    on_construct<CylinderRigidBody>().disconnect<&Scene::onRigidBodyAdded<CylinderRigidBody>>(this);
    on_construct<SphereRigidBody>().disconnect<&Scene::onRigidBodyAdded<SphereRigidBody>>(this);
    on_construct<StaticPlaneRigidBody>().disconnect<&Scene::onRigidBodyAdded<StaticPlaneRigidBody>>(this);
    on_construct<TriangleMeshRigidBody>().disconnect<&Scene::onRigidBodyAdded<TriangleMeshRigidBody>>(this);

    on_destroy<BoxRigidBody>().disconnect<&Scene::onRigidBodyRemoved<BoxRigidBody>>(this);
    on_destroy<CapsuleRigidBody>().disconnect<&Scene::onRigidBodyRemoved<CapsuleRigidBody>>(this);
    on_destroy<ConeRigidBody>().disconnect<&Scene::onRigidBodyRemoved<ConeRigidBody>>(this);
    on_destroy<CylinderRigidBody>().disconnect<&Scene::onRigidBodyRemoved<CylinderRigidBody>>(this);
    on_destroy<SphereRigidBody>().disconnect<&Scene::onRigidBodyRemoved<SphereRigidBody>>(this);
    on_destroy<StaticPlaneRigidBody>().disconnect<&Scene::onRigidBodyRemoved<StaticPlaneRigidBody>>(this);
    on_destroy<TriangleMeshRigidBody>().disconnect<&Scene::onRigidBodyRemoved<TriangleMeshRigidBody>>(this);
}

void Scene::enablePhysics() {
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

    bt_dynamics_world_->setGravity(btVector3(gravity_.x(), gravity_.y(), gravity_.x()));
}

void Scene::disablePhysics() {
    bt_dynamics_world_.reset();
    bt_sequential_impulse_constraint_solver_.reset();
    bt_overlapping_pair_cache_.reset();
    bt_collision_dispatcher_.reset();
    bt_collision_configuration_.reset();
}

void Scene::update(Duration elapsed) {
    if (bt_dynamics_world_) {
        bt_dynamics_world_->stepSimulation(durationToMicroseconds(elapsed) / 1000000.0);
    }
}

void Scene::rigidBodyAdded(RigidBody& body, Inherency* inherency) {

}

void Scene::rigidBodyRemoved(RigidBody& body) {

}

VOLCANO_WORLD_END
