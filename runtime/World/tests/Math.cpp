#include <gtest/gtest.h>

#include <Volcano/World/BoundingBox.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/World/Physics.h>
#include <Volcano/World/Mesh.h>
#include <Volcano/World/Transform.h>

namespace {

bool near(float a, float b) {
    return std::abs(a - b) < 1e-5f;
}

bool near(const Volcano::Math::Vector3f &a, const Volcano::Math::Vector3f &b) {
    return near(a.x, b.x) && near(a.y, b.y) && near(a.z, b.z);
}

} // namespace

TEST(WorldMath, Checks) {
    using namespace Volcano;

    World::BoundingBox bounds;
    EXPECT_TRUE((bounds.empty()));
    EXPECT_TRUE((bounds.center() == Math::Vector3f() && bounds.extent() == Math::Vector3f()));
    bounds.extend({-2, 1, 4});
    bounds.extend({4, -3, 8});
    EXPECT_TRUE((!bounds.empty()));
    EXPECT_TRUE((bounds.minimum() == Math::Vector3f(-2, -3, 4)));
    EXPECT_TRUE((bounds.maximum() == Math::Vector3f(4, 1, 8)));
    EXPECT_TRUE((bounds.center() == Math::Vector3f(1, -1, 6)));
    EXPECT_TRUE((bounds.extent() == Math::Vector3f(3, 2, 2)));
    static_assert(std::is_same_v<World::BoundingBox, Math::BoundingBox3f>);
    World::Mesh mesh({{{-2, 1, 4}}, {{4, -3, 8}}}, {0, 1});
    EXPECT_TRUE((mesh.bounds().minimum() == bounds.minimum()));
    EXPECT_TRUE((mesh.bounds().maximum() == bounds.maximum()));
    mesh.vertices()[0].position.x = std::numeric_limits<float>::quiet_NaN();
    bool invalidVertex = false;
    try { mesh.recalculateBounds(); }
    catch (const std::invalid_argument&) { invalidVertex = true; }
    EXPECT_TRUE((invalidVertex));
    EXPECT_TRUE((mesh.bounds().minimum() == bounds.minimum()));
    EXPECT_TRUE((mesh.bounds().maximum() == bounds.maximum()));

    const auto rotation = Math::Quaternionf::from(Math::radians(90.0f), {0, 0, 1});
    const auto joltRotation = World::toJPH(rotation);
    const auto roundTrip = World::fromJPH(joltRotation);
    EXPECT_TRUE((roundTrip == rotation));
    EXPECT_TRUE((near(roundTrip * Math::Vector3f(1, 0, 0), Math::Vector3f(0, 1, 0))));
    const Math::Vector3f position(1, -2, 3);
    EXPECT_TRUE((World::fromJPH(World::toJPH(position)) == position));

    World::Transform component;
    EXPECT_TRUE((component.affine().matrix() == Math::Matrix4f()));
    EXPECT_TRUE((component.matrix() == Math::Matrix4f()));
    component.translation() = {1, 0, 0};
    component.rotation() = rotation;
    component.scale() = {2, 3, 4};
    // Existing components apply translation, then rotation, then scaling.
    EXPECT_TRUE((near(component.matrix().transform({0, 0, 0}), Math::Vector3f(0, 3, 0))));
    EXPECT_TRUE((component.affine().matrix() == component.matrix()));
    EXPECT_TRUE((near(component.affine() * Math::Vector3f(), Math::Vector3f(0, 3, 0))));
    component.reset();
    EXPECT_TRUE((component.matrix() == Math::Matrix4f()));

    World::PerspectiveCamera perspective(90, 2, 1, 10);
    EXPECT_TRUE((near(perspective.toMatrix().transform({2, 1, -1}), Math::Vector3f(1, 1, -1))));
    EXPECT_TRUE((near(perspective.toMatrix().transform({0, 0, -10}).z, 1)));
    World::OrthographicCamera orthographic(-2, 2, -1, 1, 1, 10);
    EXPECT_TRUE((near(orthographic.toMatrix().transform({-2, -1, -1}), Math::Vector3f(-1, -1, -1))));
    EXPECT_TRUE((near(orthographic.toMatrix().transform({2, 1, -10}), Math::Vector3f(1, 1, 1))));
    perspective.aspect() = 0;
    bool caught = false;
    try { perspective.toMatrix(); }
    catch (const std::invalid_argument &) { caught = true; }
    EXPECT_TRUE((caught));

    const World::DirectionalLight direction({3, 0, 4}, {1, 1, 1});
    EXPECT_TRUE((near(direction.direction(), Math::Vector3f(0.6f, 0, 0.8f))));
    const World::DirectionalLight zeroDirection(Math::Vector3f(), {1, 1, 1});
    EXPECT_TRUE((zeroDirection.direction() == Math::Vector3f(0, -1, 0)));
}
