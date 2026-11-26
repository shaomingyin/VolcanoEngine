#include <chrono>
#include <memory>
#include <stdexcept>

#include <QCoreApplication>
#include <QQmlComponent>
#include <QQmlEngine>

#include <gtest/gtest.h>

#include <Volcano/World/Entity.h>
#include <Volcano/Physics/BoxCollisionShape.h>
#include <Volcano/Physics/RigidBody.h>
#include <Volcano/Physics/SphereCollisionShape.h>
#include <Volcano/Physics/System.h>

VOLCANO_PHYSICS_BEGIN

TEST(PhysicsSystem, CreatesBodiesFromComponentsMountedInQmlEntities) {
    int argc = 1;
    char name[] = "PhysicsTests";
    char* argv[] = { name, nullptr };
    QCoreApplication application(argc, argv);
    qmlRegisterType<World::Entity>("Volcano.World", 1, 0, "Entity");
    RigidBody::registerQmlTypes();
    System physics;
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
            "import Volcano.Physics 1.0\n"
            "Entity { RigidBody { motionType: RigidBody.Kinematic; friction: 0.4; shape: ") +
            shape + " } }", QUrl());
        ASSERT_TRUE(component.isReady()) << component.errorString().toStdString();
        const std::unique_ptr<QObject> object(component.create());
        const auto* entity = qobject_cast<World::Entity*>(object.get());
        ASSERT_NE(entity, nullptr) << component.errorString().toStdString();
        ASSERT_EQ(entity->components().size(), 1);
        const auto* body = qobject_cast<RigidBody*>(entity->components().first());
        ASSERT_NE(body, nullptr);
        EXPECT_EQ(body->motionType(), RigidBody::MotionType::Kinematic);
        EXPECT_FLOAT_EQ(body->friction(), 0.4f);

        physics.enable();
        const QVector3D position(1.0f, 2.0f, 3.0f);
        const QQuaternion rotation(0.0f, 0.0f, 0.0f, 1.0f);
        const auto id = physics.createBody(*body, position, rotation);
        const auto pose = physics.bodyPose(id);
        EXPECT_EQ(pose.position, position);
        EXPECT_EQ(pose.rotation, rotation);

        EXPECT_FALSE(physics.isBodyAdded(id));
        physics.addBody(id, false);
        EXPECT_TRUE(physics.isBodyAdded(id));
        physics.removeBody(id);
        EXPECT_FALSE(physics.isBodyAdded(id));
        physics.destroyBody(id);
    }
}

TEST(PhysicsSystem, BodiesKeepTheirShapeAfterConfigurationChangesAndDestruction) {
    System physics;
    physics.enable();
    System::BodyId groundId;
    System::BodyId original;
    System::BodyId updated;

    {
        BoxCollisionShape groundShape;
        groundShape.setSize(QVector3D(8.0f, 1.0f, 8.0f));
        RigidBody ground;
        ground.setShape(&groundShape);
        ground.setMotionType(RigidBody::MotionType::Static);
        ground.setObjectLayer(0);
        groundId = physics.createBody(ground, QVector3D(0.0f, -0.5f, 0.0f));

        SphereCollisionShape sphere;
        RigidBody body;
        body.setShape(&sphere);
        original = physics.createBody(body, QVector3D(-2.0f, 2.0f, 0.0f));

        sphere.setRadius(0.75f);
        updated = physics.createBody(body, QVector3D(2.0f, 2.0f, 0.0f));
    }

    physics.addBody(groundId);
    physics.addBody(original);
    physics.addBody(updated);
    for (int frame = 0; frame < 120; ++frame) {
        physics.update(std::chrono::milliseconds(16));
    }
    EXPECT_NEAR(physics.bodyPose(original).position.y(), 0.5f, 0.05f);
    EXPECT_NEAR(physics.bodyPose(updated).position.y(), 0.75f, 0.05f);
    physics.destroyBody(updated);
    physics.destroyBody(original);
    physics.destroyBody(groundId);
}

TEST(PhysicsSystem, CreateBodyRejectsDisabledPhysicsAndInvalidConfiguration) {
    BoxCollisionShape box;
    box.setConvexRadius(0.5f);
    EXPECT_FALSE(box.isValid());
    EXPECT_FLOAT_EQ(box.convexRadius(), 0.5f);
    System physics;
    RigidBody body;
    EXPECT_THROW(physics.createBody(body), std::runtime_error);
    physics.enable();
    EXPECT_THROW(physics.createBody(body), std::invalid_argument);
    body.setShape(&box);
    EXPECT_THROW(physics.createBody(body), std::invalid_argument);

    box.setConvexRadius(0.05f);
    body.setObjectLayer(2);
    EXPECT_THROW(physics.createBody(body), std::invalid_argument);
}

TEST(PhysicsSystem, ReportsJoltShapeCreationFailure) {
    class FailingCollisionShape final: public CollisionShape {
    public:
        bool isValid() const noexcept override {
            return true;
        }

    private:
        const JPH::SphereShapeSettings& settings() const noexcept override {
            return settings_;
        }

    private:
        JPH::SphereShapeSettings settings_ { 0.0f };
    };

    System physics;
    physics.enable();
    FailingCollisionShape shape;
    RigidBody body;
    body.setShape(&shape);
    EXPECT_THROW(physics.createBody(body), std::runtime_error);
}

TEST(PhysicsSystem, RejectsDestroyedBodiesAndIdsFromOtherSystemGenerations) {
    System physics;
    System otherPhysics;
    physics.enable();
    otherPhysics.enable();
    SphereCollisionShape sphere;
    RigidBody body;
    body.setShape(&sphere);
    const auto id = physics.createBody(body);
    const auto otherId = otherPhysics.createBody(body);
    EXPECT_NE(id, otherId);
    EXPECT_THROW(otherPhysics.destroyBody(id), std::invalid_argument);

    physics.addBody(id);
    physics.destroyBody(id);
    EXPECT_THROW(physics.destroyBody(id), std::invalid_argument);
    const auto beforeRestart = physics.createBody(body);
    physics.disable();
    physics.enable();
    const auto afterRestart = physics.createBody(body);
    EXPECT_NE(beforeRestart, afterRestart);
    EXPECT_THROW(physics.bodyPose(beforeRestart), std::invalid_argument);
    EXPECT_EQ(physics.bodyPose(afterRestart).position, QVector3D());
}

VOLCANO_PHYSICS_END
