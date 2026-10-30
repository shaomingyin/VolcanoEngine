#include <limits>
#include <type_traits>

#include <gtest/gtest.h>

#include <Volcano/Math.h>
#include <Volcano/Transform.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>

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

    World::PerspectiveCamera perspective(90, 2, 1, 10);
    static_assert(std::is_same_v<decltype(perspective.offset()), const Volcano::Transform&>);
    EXPECT_TRUE((near((perspective.toMatrix() * Eigen::Vector4f(2, 1, -1, 1)).hnormalized(), Eigen::Vector3f(1, 1, -1))));
    EXPECT_TRUE((near((perspective.toMatrix() * Eigen::Vector4f(0, 0, -10, 1)).hnormalized().z(), 1)));
    World::OrthographicCamera orthographic(-2, 2, -1, 1, 1, 10);
    EXPECT_TRUE((near((orthographic.toMatrix() * Eigen::Vector4f(-2, -1, -1, 1)).hnormalized(), Eigen::Vector3f(-1, -1, -1))));
    EXPECT_TRUE((near((orthographic.toMatrix() * Eigen::Vector4f(2, 1, -10, 1)).hnormalized(), Eigen::Vector3f(1, 1, 1))));
    EXPECT_THROW(perspective.setAspect(0), std::invalid_argument);

    const World::DirectionalLight direction({3, 0, 4}, {1, 1, 1});
    EXPECT_TRUE((near(direction.direction(), Eigen::Vector3f(0.6f, 0, 0.8f))));
    EXPECT_THROW((World::DirectionalLight(Eigen::Vector3f::Zero(), {1, 1, 1})), std::invalid_argument);
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
