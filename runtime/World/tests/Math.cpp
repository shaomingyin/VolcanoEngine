#include <limits>
#include <type_traits>

#include <gtest/gtest.h>

#include <Volcano/Math.h>
#include <Volcano/World/BoundingBox.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/World/Physics.h>
#include <Volcano/Transform.h>

namespace {

bool near(float a, float b) {
    return std::abs(a - b) < 1e-5f;
}

bool near(const Eigen::Vector3f &a, const Eigen::Vector3f &b) {
    return near(a.x(), b.x()) && near(a.y(), b.y()) && near(a.z(), b.z());
}

} // namespace

TEST(WorldMath, Checks) {
    namespace World = Volcano::World;

    World::BoundingBox bounds;
    EXPECT_TRUE((bounds.isEmpty()));
    bounds.extend(Eigen::Vector3f(-2, 1, 4));
    bounds.extend(Eigen::Vector3f(4, -3, 8));
    EXPECT_TRUE((!bounds.isEmpty()));
    EXPECT_TRUE((bounds.min() == Eigen::Vector3f(-2, -3, 4)));
    EXPECT_TRUE((bounds.max() == Eigen::Vector3f(4, 1, 8)));
    EXPECT_TRUE((bounds.center() == Eigen::Vector3f(1, -1, 6)));
    EXPECT_TRUE(((bounds.sizes() / 2.0f) == Eigen::Vector3f(3, 2, 2)));
    static_assert(std::is_same_v<World::BoundingBox, Eigen::AlignedBox3f>);
    const Eigen::Quaternionf rotation(Eigen::AngleAxisf(Volcano::Eigen::radians(90.0f), Eigen::Vector3f::UnitZ()));
    const auto joltRotation = World::toJPH(rotation);
    const auto roundTrip = World::fromJPH(joltRotation);
    EXPECT_TRUE((roundTrip.coeffs() == rotation.coeffs()));
    EXPECT_TRUE((near(roundTrip * Eigen::Vector3f(1, 0, 0), Eigen::Vector3f(0, 1, 0))));
    const Eigen::Vector3f position(1, -2, 3);
    EXPECT_TRUE((World::fromJPH(World::toJPH(position)) == position));

    World::PerspectiveCamera perspective(90, 2, 1, 10);
    static_assert(std::is_same_v<decltype(perspective.offset()), Volcano::Transform&>);
    EXPECT_TRUE((near((perspective.toMatrix() * Eigen::Vector4f(2, 1, -1, 1)).hnormalized(), Eigen::Vector3f(1, 1, -1))));
    EXPECT_TRUE((near((perspective.toMatrix() * Eigen::Vector4f(0, 0, -10, 1)).hnormalized().z(), 1)));
    World::OrthographicCamera orthographic(-2, 2, -1, 1, 1, 10);
    EXPECT_TRUE((near((orthographic.toMatrix() * Eigen::Vector4f(-2, -1, -1, 1)).hnormalized(), Eigen::Vector3f(-1, -1, -1))));
    EXPECT_TRUE((near((orthographic.toMatrix() * Eigen::Vector4f(2, 1, -10, 1)).hnormalized(), Eigen::Vector3f(1, 1, 1))));
    perspective.aspect() = 0;
    bool caught = false;
    try { perspective.toMatrix(); }
    catch (const std::invalid_argument &) { caught = true; }
    EXPECT_TRUE((caught));

    const World::DirectionalLight direction({3, 0, 4}, {1, 1, 1});
    EXPECT_TRUE((near(direction.direction(), Eigen::Vector3f(0.6f, 0, 0.8f))));
    const World::DirectionalLight zeroDirection(Eigen::Vector3f::Zero(), {1, 1, 1});
    EXPECT_TRUE((zeroDirection.direction() == Eigen::Vector3f(0, -1, 0)));
}

TEST(WorldMath, RejectsInvalidProjectionParameters) {
    using namespace Volcano::World;
    const float nan = std::numeric_limits<float>::quiet_NaN();
    const float inf = std::numeric_limits<float>::infinity();
    for (const float fov : {0.0f, -1.0f, 180.0f, nan, inf}) {
        EXPECT_THROW(PerspectiveCamera(fov, 1, 1, 10).toMatrix(), std::invalid_argument);
    }
    for (const float aspect : {0.0f, -1.0f, nan, inf}) {
        EXPECT_THROW(PerspectiveCamera(90, aspect, 1, 10).toMatrix(), std::invalid_argument);
    }
    for (const float nearPlane : {0.0f, -1.0f, 10.0f, nan, inf}) {
        EXPECT_THROW(PerspectiveCamera(90, 1, nearPlane, 10).toMatrix(), std::invalid_argument);
    }
    for (const float farPlane : {0.0f, 1.0f, nan, inf}) {
        EXPECT_THROW(PerspectiveCamera(90, 1, 1, farPlane).toMatrix(), std::invalid_argument);
    }
    for (int i = 0; i < 6; ++i) {
        for (const float bad : {nan, inf}) {
            float values[] = {-2, 2, -1, 1, 1, 10};
            values[i] = bad;
            EXPECT_THROW(OrthographicCamera(values[0], values[1], values[2], values[3], values[4], values[5]).toMatrix(), std::invalid_argument);
        }
    }
    EXPECT_THROW(OrthographicCamera(1, 1, -1, 1, 1, 10).toMatrix(), std::invalid_argument);
    EXPECT_THROW(OrthographicCamera(-1, 1, 1, 1, 1, 10).toMatrix(), std::invalid_argument);
    EXPECT_THROW(OrthographicCamera(-1, 1, -1, 1, 1, 1).toMatrix(), std::invalid_argument);
    EXPECT_TRUE(OrthographicCamera(1, -1, 1, -1, 10, 1).toMatrix().allFinite());
}
