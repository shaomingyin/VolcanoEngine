#include <chrono>
#include <limits>
#include <numbers>
#include <stdexcept>

#include <gtest/gtest.h>

#include <Volcano/World/RigidBody.h>
#include <Volcano/Physics/System.h>

namespace {
using namespace Volcano;
using Body = World::RigidBody;
using namespace std::chrono_literals;

TEST(PhysicsSystem, BindsDescriptionsAndDisconnectsOnDestruction) {
    entt::registry registry;
    const auto entity = registry.create();
    registry.emplace<Transform>(entity);
    registry.emplace<Body>(entity).setShape(Body::Sphere{});
    {
        Physics::System physics(registry);
        EXPECT_TRUE(physics.contains(entity));
        physics.update(16ms);
        EXPECT_LT(registry.get<Transform>(entity).translation().y(), 0.0f);
        const Eigen::Vector3f position = registry.get<Transform>(entity).translation();
        physics.update({});
        EXPECT_EQ(registry.get<Transform>(entity).translation(), position);
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
        EXPECT_DEATH(physics.update(-1ms), "[Aa]ssert");
#endif
        registry.remove<Transform>(entity);
        EXPECT_FALSE(physics.contains(entity));
        registry.emplace<Transform>(entity);
        EXPECT_FALSE(physics.contains(entity));
        physics.synchronize();
        EXPECT_TRUE(physics.contains(entity));
    }
    EXPECT_TRUE((registry.all_of<Body, Transform>(entity)));
    registry.patch<Body>(entity, [](auto& body) {
        body.setMass(2.0f);
    });
    {
        Physics::System physics(registry);
        EXPECT_TRUE(physics.contains(entity));
        registry.destroy(entity);
        EXPECT_FALSE(physics.contains(entity));
    }
}

TEST(PhysicsSystem, RejectsInvalidReplacementAndPreservesMotionOnSuccessfulReplacement) {
    entt::registry registry;
    const auto entity = registry.create();
    registry.emplace<Transform>(entity);
    registry.emplace<Body>(entity).setLinearDamping(0.0f);
    Physics::System physics(registry);
    physics.update(16ms);
    const float before = registry.get<Transform>(entity).translation().y();
    registry.patch<Transform>(entity, [](auto& transform) {
        transform.setScale(Eigen::Vector3f::Constant(2.0f));
    });
    EXPECT_THROW(physics.synchronize(), std::invalid_argument);
    EXPECT_TRUE(physics.contains(entity));
    EXPECT_EQ(registry.get<Transform>(entity).translation().y(), before);
    registry.patch<Transform>(entity, [](auto& transform) {
        transform.setScale(Eigen::Vector3f::Ones());
    });
    registry.patch<Body>(entity, [](auto& body) {
        body.setShape(Body::Sphere{0.5f});
        body.setGravityFactor(0.0f);
    });
    physics.synchronize();
    physics.update(16ms);
    EXPECT_NEAR(registry.get<Transform>(entity).translation().y() - before, before, 1e-5f);
    registry.remove<Body>(entity);
    EXPECT_FALSE(physics.contains(entity));
}

TEST(PhysicsSystem, SupportsAllShapesAndStaticAndKinematicMotion) {
    entt::registry registry;
    const auto floor = registry.create();
    registry.emplace<Transform>(floor).setTranslation(Eigen::Vector3f(0, -0.5f, 0));
    auto& ground = registry.emplace<Body>(floor);
    ground.setMotion(Body::Motion::Static);
    ground.setShape(Body::Box{Eigen::Vector3f(10, 0.5f, 10)});
    const auto sphere = registry.create();
    registry.emplace<Transform>(sphere).setTranslation(Eigen::Vector3f(-2, 2, 0));
    registry.emplace<Body>(sphere).setShape(Body::Sphere{0.5f});
    const auto capsule = registry.create();
    registry.emplace<Transform>(capsule).setTranslation(Eigen::Vector3f(2, 2, 0));
    registry.emplace<Body>(capsule).setShape(Body::Capsule{0.5f, 0.5f});
    const auto platform = registry.create();
    registry.emplace<Transform>(platform).setTranslation(Eigen::Vector3f(20, 0, 0));
    registry.emplace<Body>(platform).setMotion(Body::Motion::Kinematic);
    Physics::System physics(registry);
    registry.patch<Transform>(platform, [](auto& transform) {
        transform.setTranslation(Eigen::Vector3f(20, 1, 0));
    });
    physics.update(16ms);
    EXPECT_NEAR(registry.get<Transform>(platform).translation().y(), 1.0f, 1e-4f);
    for (int i = 0; i < 180; ++i) {
        physics.update(16ms);
    }
    EXPECT_NEAR(registry.get<Transform>(floor).translation().y(), -0.5f, 1e-5f);
    EXPECT_NEAR(registry.get<Transform>(sphere).translation().y(), 0.5f, 0.05f);
    EXPECT_NEAR(registry.get<Transform>(capsule).translation().y(), 1.0f, 0.05f);
}

TEST(PhysicsSystem, OffsetShapesCollideWithoutChangingTheEntityOriginConvention) {
    entt::registry registry;
    const auto floor = registry.create();
    registry.emplace<Transform>(floor);
    auto& ground = registry.emplace<Body>(floor);
    ground.setMotion(Body::Motion::Static);
    ground.setShape(Body::Box{Eigen::Vector3f(0.5f, 10, 10)});
    Transform floorOffset;
    floorOffset.setTranslation({0, -0.5f, 0});
    const Eigen::Quaternionf quarterTurn(Eigen::AngleAxisf(
        std::numbers::pi_v<float> / 2.0f, Eigen::Vector3f::UnitZ()));
    floorOffset.setRotation(quarterTurn);
    ground.setOffset(floorOffset);

    const auto entity = registry.create();
    auto& transform = registry.emplace<Transform>(entity);
    transform.setTranslation({0, 4, 0});
    transform.setRotation(quarterTurn);
    auto& body = registry.emplace<Body>(entity);
    body.setShape(Body::Sphere{0.5f});
    Transform offset;
    offset.setTranslation({2, 0, 0});
    body.setOffset(offset);

    Physics::System physics(registry);
    for (int i = 0; i < 240; ++i) {
        physics.update(16ms);
    }
    EXPECT_NEAR(registry.get<Transform>(entity).translation().y(), -1.5f, 0.05f);
    EXPECT_TRUE(registry.get<Transform>(floor).translation().isZero(1e-5f));
    EXPECT_TRUE(registry.get<Transform>(entity).rotation().isApprox(quarterTurn, 1e-3f));
    EXPECT_EQ(registry.get<Body>(entity).offset().translation(), Eigen::Vector3f(2, 0, 0));
}

TEST(PhysicsSystem, KinematicOffsetAndReplacementPreserveEntityTargets) {
    entt::registry registry;
    const auto entity = registry.create();
    registry.emplace<Transform>(entity);
    auto& body = registry.emplace<Body>(entity);
    body.setMotion(Body::Motion::Kinematic);
    Transform offset;
    offset.setTranslation({2, 1, 0});
    body.setOffset(offset);
    Physics::System physics(registry);
    const Eigen::Vector3f target(3, 4, 5);
    const Eigen::Quaternionf rotation(Eigen::AngleAxisf(0.7f, Eigen::Vector3f::UnitZ()));
    registry.patch<Transform>(entity, [&](auto& transform) {
        transform.setTranslation(target);
        transform.setRotation(rotation);
    });
    physics.update(16ms);
    EXPECT_TRUE(registry.get<Transform>(entity).translation().isApprox(target, 1e-4f));
    EXPECT_TRUE(registry.get<Transform>(entity).rotation().isApprox(rotation, 1e-4f));
    offset.setTranslation({-1, 2, 0});
    registry.patch<Body>(entity, [&](auto& description) {
        description.setOffset(offset);
    });
    physics.update(16ms);
    EXPECT_TRUE(registry.get<Transform>(entity).translation().isApprox(target, 1e-4f));
    EXPECT_TRUE(registry.get<Transform>(entity).rotation().isApprox(rotation, 1e-4f));
}

TEST(PhysicsSystem, InvalidShapeOffsetPreservesTheBodyAndCanRetry) {
    entt::registry registry;
    const auto entity = registry.create();
    registry.emplace<Transform>(entity).setTranslation({1, 2, 3});
    registry.emplace<Body>(entity).setMotion(Body::Motion::Static);
    Physics::System physics(registry);
    Transform offset;
    for (int invalid = 0; invalid < 3; ++invalid) {
        offset.reset();
        if (invalid == 0) {
            offset.setScale({2, 1, 1});
        } else if (invalid == 1) {
            offset.setRotation(Eigen::Quaternionf(0, 0, 0, 0));
        } else {
            offset.setTranslation({std::numeric_limits<float>::quiet_NaN(), 0, 0});
        }
        registry.patch<Body>(entity, [&](auto& description) {
            description.setOffset(offset);
        });
        EXPECT_THROW(physics.synchronize(), std::invalid_argument);
        EXPECT_TRUE(physics.contains(entity));
        EXPECT_EQ(registry.get<Transform>(entity).translation(), Eigen::Vector3f(1, 2, 3));
    }
    offset.reset();
    offset.setTranslation({0, 1, 0});
    registry.patch<Body>(entity, [&](auto& description) {
        description.setOffset(offset);
    });
    EXPECT_NO_THROW(physics.synchronize());
    physics.update(16ms);
    EXPECT_EQ(registry.get<Transform>(entity).translation(), Eigen::Vector3f(1, 2, 3));
}

TEST(PhysicsSystem, ValidatesTransformOutsideRegistryCallbacksAndCanRetry) {
    entt::registry registry;
    Physics::System physics(registry);
    const auto entity = registry.create();
    registry.emplace<Transform>(entity);
    registry.emplace<Body>(entity);
    registry.get<Transform>(entity).setScale(Eigen::Vector3f(2, 1, 1));
    EXPECT_THROW(physics.synchronize(), std::invalid_argument);
    registry.get<Transform>(entity).reset();
    registry.get<Transform>(entity).setRotation(Eigen::Quaternionf(0, 0, 0, 0));
    EXPECT_THROW(physics.synchronize(), std::invalid_argument);
    registry.get<Transform>(entity).reset();
    EXPECT_NO_THROW(physics.synchronize());
    EXPECT_TRUE(physics.contains(entity));
}

TEST(PhysicsSystem, ConstructionFailureReleasesBodiesAndRuntime) {
    entt::registry registry;
    const auto entity = registry.create();
    registry.emplace<Transform>(entity);
    registry.emplace<Body>(entity);
    registry.get<Transform>(entity).setScale(Eigen::Vector3f::Constant(2));
    const auto valid = registry.create();
    registry.emplace<Transform>(valid).setTranslation(Eigen::Vector3f(20, 0, 0));
    registry.emplace<Body>(valid);
    EXPECT_THROW(Physics::System{registry}, std::invalid_argument);
    registry.get<Transform>(entity).reset();
    {
        Physics::System physics(registry);
        EXPECT_TRUE(physics.contains(entity));
        const Eigen::Quaternionf rotation(Eigen::AngleAxisf(0.7f, Eigen::Vector3f::UnitZ()));
        registry.patch<Transform>(entity, [&](auto& transform) {
            transform.setRotation(rotation);
        });
        physics.update(1ns);
        EXPECT_TRUE(registry.get<Transform>(entity).rotation().isApprox(rotation));
    }
    EXPECT_NO_THROW(registry.destroy(entity));
}
}
