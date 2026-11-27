#include <chrono>
#include <memory>

#include <QCoreApplication>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QVariant>

#include <gtest/gtest.h>

#include <Volcano/Physics/System.h>
#include <Volcano/World/BoxCollisionShape.h>
#include <Volcano/World/RigidBody.h>
#include <Volcano/World/Scene.h>
#include <Volcano/World/SphereCollisionShape.h>

VOLCANO_PHYSICS_BEGIN

using World::BoxCollisionShape;
using World::CollisionShape;
using World::RigidBody;
using World::SphereCollisionShape;

namespace {
constexpr char binding[] = "volcano.physics.bodyBinding";
}

TEST(PhysicsSystem, CreatesBodiesFromComponentsMountedInQmlEntities) {
    int argc = 1;
    char name[] = "PhysicsTests";
    char* argv[] = { name, nullptr };
    QCoreApplication application(argc, argv);
    World::Scene::registerQmlTypes();
    EXPECT_EQ(System::staticMetaObject.indexOfProperty("enabled"), -1);
    EXPECT_EQ(System::staticMetaObject.indexOfProperty("gravity"), -1);
    EXPECT_EQ(System::staticMetaObject.indexOfMethod("enable()"), -1);
    EXPECT_EQ(System::staticMetaObject.indexOfMethod("disable()"), -1);
    QQmlEngine engine;

    const char* const shapes[] = {
        "BoxCollisionShape { convexRadius: 0.1 }",
        "CapsuleCollisionShape { radius: 0.35; cylinderHeight: 1.2 }",
        "SphereCollisionShape { radius: 0.75 }",
    };

    for (const auto* shape: shapes) {
        SCOPED_TRACE(shape);
        QQmlComponent component(&engine);
        component.setData(QByteArray(
            "import Volcano.World 1.0\n"
            "Scene { Entity { Model {}\n"
            "RigidBody { motionType: RigidBody.Kinematic; friction: 0.4; shape: ") +
            shape + " } } }", QUrl());
        ASSERT_TRUE(component.isReady()) << component.errorString().toStdString();
        const QVector3D gravity(0.0f, -5.0f, 0.0f);
        const std::unique_ptr<QObject> object(component.createWithInitialProperties(
            {{QStringLiteral("gravity"), QVariant::fromValue(gravity)}}));
        auto* scene = qobject_cast<World::Scene*>(object.get());
        ASSERT_NE(scene, nullptr) << component.errorString().toStdString();
        EXPECT_EQ(scene->gravity(), gravity);
        ASSERT_EQ(scene->entityCount(), 1);
        auto* entity = scene->entityAt(0);
        ASSERT_EQ(entity->components().size(), 2);
        auto* body = qobject_cast<RigidBody*>(entity->components().last());
        ASSERT_NE(body, nullptr);
        EXPECT_EQ(body->motionType(), RigidBody::MotionType::Kinematic);
        EXPECT_FLOAT_EQ(body->friction(), 0.4f);

        const QVector3D position(1.0f, 2.0f, 3.0f);
        const QQuaternion rotation(0.0f, 0.0f, 0.0f, 1.0f);
        entity->transform()->setTranslation(position);
        entity->transform()->setRotation(rotation);
        auto physics = std::make_unique<System>(*scene);
        EXPECT_FALSE(physics->isEnabled());
        physics->enable();
        EXPECT_TRUE(body->property(binding).isValid());
        const auto kinematicPosition = position + QVector3D(0.0f, 1.0f, 0.0f);
        entity->transform()->setTranslation(kinematicPosition);
        physics->update(std::chrono::milliseconds(16));
        EXPECT_EQ(entity->transform()->translation(), kinematicPosition);
        entity->removeLastComponent();
        EXPECT_FALSE(body->property(binding).isValid());
        entity->appendComponent(body);
        EXPECT_TRUE(body->property(binding).isValid());

        auto temporaryBody = std::make_unique<RigidBody>();
        temporaryBody->setShape(body->shape());
        entity->appendComponent(temporaryBody.get());
        EXPECT_TRUE(temporaryBody->property(binding).isValid());
        temporaryBody.reset();
        EXPECT_EQ(entity->componentCount(), 2);
        auto temporaryEntity = std::make_unique<World::Entity>();
        scene->appendEntity(temporaryEntity.get());
        temporaryEntity.reset();
        EXPECT_EQ(scene->entityCount(), 1);

        physics->disable();
        EXPECT_FALSE(body->property(binding).isValid());
        physics->enable();
        EXPECT_TRUE(body->property(binding).isValid());
        physics.reset();
        EXPECT_FALSE(body->property(binding).isValid());
    }
}

TEST(PhysicsSystem, BodiesKeepTheirShapeAfterShapeChangesAndDestruction) {
    World::Scene scene;
    World::Entity groundEntity;
    World::Entity originalEntity;
    World::Entity updatedEntity;
    scene.appendEntity(&groundEntity);
    scene.appendEntity(&originalEntity);
    scene.appendEntity(&updatedEntity);
    System physics(scene);
    physics.enable();
    auto ground = std::make_unique<RigidBody>();
    RigidBody original;
    RigidBody updated;

    {
        BoxCollisionShape groundShape;
        groundShape.setSize(QVector3D(8.0f, 1.0f, 8.0f));
        ground->setShape(&groundShape);
        ground->setMotionType(RigidBody::MotionType::Static);
        ground->setObjectLayer(0);
        groundEntity.transform()->setTranslation(QVector3D(0.0f, -0.5f, 0.0f));
        groundEntity.appendComponent(ground.get());

        SphereCollisionShape sphere;
        original.setShape(&sphere);
        updated.setShape(&sphere);
        originalEntity.transform()->setTranslation(QVector3D(-2.0f, 2.0f, 0.0f));
        originalEntity.appendComponent(&original);

        sphere.setRadius(0.75f);
        updatedEntity.transform()->setTranslation(QVector3D(2.0f, 2.0f, 0.0f));
        updatedEntity.appendComponent(&updated);
    }

    for (int frame = 0; frame < 120; ++frame) {
        physics.update(std::chrono::milliseconds(16));
    }
    EXPECT_NEAR(originalEntity.transform()->translation().y(), 0.5f, 0.05f);
    EXPECT_NEAR(updatedEntity.transform()->translation().y(), 0.75f, 0.05f);
    ground.reset();
    // Use a fresh falling body to check that destruction removed the ground.
    SphereCollisionShape sphere;
    originalEntity.removeLastComponent();
    originalEntity.transform()->setTranslation(QVector3D(-2.0f, 2.0f, 0.0f));
    original.setShape(&sphere);
    originalEntity.appendComponent(&original);
    for (int frame = 0; frame < 60; ++frame) {
        physics.update(std::chrono::milliseconds(16));
    }
    EXPECT_LT(originalEntity.transform()->translation().y(), 0.0f);
}

TEST(PhysicsSystem, UsesSceneGravityAtStartupAndWhenItChanges) {
    World::Scene scene;
    EXPECT_EQ(scene.gravity(), QVector3D(0.0f, -9.81f, 0.0f));
    scene.setGravity(QVector3D());
    World::Entity entity;
    const QQuaternion rotation(0.0f, 0.0f, 0.0f, 1.0f);
    entity.transform()->setRotation(rotation);
    const QVector3D scale(2.0f, 3.0f, 4.0f);
    entity.transform()->setScale(scale);
    SphereCollisionShape shape;
    RigidBody body;
    body.setShape(&shape);
    entity.appendComponent(&body);
    scene.appendEntity(&entity);
    System physics(scene);
    physics.enable();

    entity.transform()->setRotation(QQuaternion());
    physics.update(std::chrono::milliseconds(100));
    EXPECT_EQ(entity.transform()->translation(), QVector3D());
    EXPECT_EQ(entity.transform()->rotation(), rotation);
    EXPECT_EQ(entity.transform()->scale(), scale);

    scene.setGravity(QVector3D(0.0f, -10.0f, 0.0f));
    physics.update(std::chrono::milliseconds(100));
    EXPECT_LT(entity.transform()->translation().y(), 0.0f);
}

TEST(PhysicsSystem, SkipsInvalidSceneBodies) {
    BoxCollisionShape box;
    box.setConvexRadius(0.5f);
    EXPECT_FALSE(box.isValid());
    EXPECT_FLOAT_EQ(box.convexRadius(), 0.5f);
    World::Scene scene;
    World::Entity entity;
    scene.appendEntity(&entity);
    System physics(scene);
    RigidBody body;
    entity.appendComponent(&body);
    EXPECT_FALSE(body.property(binding).isValid());
    physics.enable();
    EXPECT_FALSE(body.property(binding).isValid());
    entity.removeLastComponent();
    body.setShape(&box);
    entity.appendComponent(&body);
    EXPECT_FALSE(body.property(binding).isValid());

    entity.removeLastComponent();
    box.setConvexRadius(0.05f);
    body.setObjectLayer(2);
    entity.appendComponent(&body);
    EXPECT_FALSE(body.property(binding).isValid());

    entity.removeLastComponent();
    body.setObjectLayer(1);
    entity.appendComponent(&body);
    EXPECT_TRUE(body.property(binding).isValid());
}

TEST(PhysicsSystem, RejectsUnsupportedSceneShape) {
    class UnsupportedShape final: public CollisionShape {
    public:
        bool isValid() const noexcept override {
            return true;
        }
    };

    World::Scene scene;
    World::Entity entity;
    scene.appendEntity(&entity);
    System physics(scene);
    physics.enable();
    UnsupportedShape shape;
    RigidBody body;
    body.setShape(&shape);
    entity.appendComponent(&body);
    EXPECT_FALSE(body.property(binding).isValid());
}

TEST(PhysicsSystem, UpdatesTransformsOnlyInItsBoundScene) {
    World::Scene scene;
    World::Scene otherScene;
    World::Entity entity;
    scene.appendEntity(&entity);
    System physics(scene);
    System otherPhysics(otherScene);
    physics.enable();
    otherPhysics.enable();
    SphereCollisionShape sphere;
    RigidBody body;
    body.setShape(&sphere);
    entity.appendComponent(&body);
    const auto step = std::chrono::milliseconds(16);
    otherPhysics.update(step);
    EXPECT_EQ(entity.transform()->translation(), QVector3D());
    physics.update(step);
    const auto position = entity.transform()->translation();
    EXPECT_LT(position.y(), 0.0f);
    physics.disable();
    physics.update(step);
    EXPECT_EQ(entity.transform()->translation(), position);
    physics.enable();
    scene.removeLastEntity();
    EXPECT_FALSE(body.property(binding).isValid());
    otherScene.appendEntity(&entity);
    physics.update(step);
    EXPECT_EQ(entity.transform()->translation(), position);
    otherPhysics.update(step);
    EXPECT_LT(entity.transform()->translation().y(), position.y());
}

TEST(PhysicsSystem, SkipsWritebackForEntitiesRemovedByTransformNotifications) {
    World::Scene scene;
    World::Entity first;
    World::Entity second;
    SphereCollisionShape sphere;
    RigidBody firstBody;
    RigidBody secondBody;
    firstBody.setShape(&sphere);
    secondBody.setShape(&sphere);
    first.appendComponent(&firstBody);
    second.appendComponent(&secondBody);
    const QVector3D secondPosition(3.0f, 0.0f, 0.0f);
    second.transform()->setTranslation(secondPosition);
    scene.appendEntity(&first);
    scene.appendEntity(&second);
    System physics(scene);
    physics.enable();

    QObject::connect(first.transform(), &World::Transform::translationChanged, &scene, [&scene] {
        scene.removeLastEntity();
    });
    physics.update(std::chrono::milliseconds(16));
    EXPECT_EQ(scene.entityCount(), 1);
    EXPECT_EQ(second.transform()->translation(), secondPosition);
}

VOLCANO_PHYSICS_END
