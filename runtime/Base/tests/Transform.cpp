#include <limits>
#include <numbers>
#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include <Volcano/Transform.h>

using Volcano::Transform;

TEST(BaseTransform, DefaultsToIdentity) {
    const Transform transform;
    EXPECT_TRUE(transform.translation().isZero());
    EXPECT_TRUE(transform.scale().isOnes());
    EXPECT_TRUE(transform.rotation().isApprox(Eigen::Quaternionf::Identity()));
    EXPECT_TRUE(transform.affine().matrix().isIdentity());
    EXPECT_TRUE(transform.normalMatrix().isIdentity());

    static_assert(std::is_same_v<decltype(transform.affine()), Eigen::Affine3f>);
    static_assert(std::is_same_v<decltype(transform.translation()), const Eigen::Vector3f&>);
    static_assert(std::is_same_v<decltype(transform.scale()), const Eigen::Vector3f&>);
    static_assert(std::is_same_v<decltype(transform.rotation()), const Eigen::Quaternionf&>);
}

TEST(BaseTransform, ConstructsComponentsAndAppliesTranslationThenRotationThenScale) {
    const Eigen::Vector3f translation(1, -2, 3);
    const Eigen::Vector3f scale(2, 3, 4);
    const Eigen::Quaternionf rotation(Eigen::AngleAxisf(std::numbers::pi_v<float> / 2,
        Eigen::Vector3f::UnitZ()));
    const Transform transform(translation, scale, rotation);

    EXPECT_EQ(transform.translation(), translation);
    EXPECT_EQ(transform.scale(), scale);
    EXPECT_TRUE(transform.rotation().isApprox(rotation));

    const auto affine = transform.affine();
    EXPECT_TRUE((affine * Eigen::Vector3f(4, 5, 6)).isApprox(Eigen::Vector3f(-6, 15, 36)));
    EXPECT_TRUE((affine * Eigen::Vector3f::Zero()).isApprox(Eigen::Vector3f(4, 3, 12)));
}

TEST(BaseTransform, EditingAndResetDoNotChangePreviouslyGeneratedAffine) {
    Transform transform;
    transform.translation() = {1, 2, 3};
    const auto previous = transform.affine();

    transform.translation().x() = 4;
    transform.scale() = {2, 3, 4};
    transform.rotation() = Eigen::AngleAxisf(std::numbers::pi_v<float> / 2, Eigen::Vector3f::UnitZ());
    const Transform copy = transform;
    EXPECT_TRUE((previous * Eigen::Vector3f::Zero()).isApprox(Eigen::Vector3f(1, 2, 3)));
    EXPECT_TRUE((transform.affine() * Eigen::Vector3f::Zero()).isApprox(Eigen::Vector3f(-4, 12, 12)));

    transform.reset();
    EXPECT_TRUE(transform.translation().isZero());
    EXPECT_TRUE(transform.scale().isOnes());
    EXPECT_TRUE(transform.rotation().isApprox(Eigen::Quaternionf::Identity()));
    EXPECT_TRUE(transform.affine().matrix().isIdentity());
    EXPECT_TRUE((copy.affine() * Eigen::Vector3f::Zero()).isApprox(Eigen::Vector3f(-4, 12, 12)));
}

TEST(BaseTransform, AllowsNegativeAndZeroScale) {
    const Transform transform({1, 0, 0}, {-2, 0, 3}, Eigen::Quaternionf::Identity());
    const auto affine = transform.affine();

    EXPECT_TRUE(affine.matrix().allFinite());
    EXPECT_TRUE((affine * Eigen::Vector3f(1, 2, 3)).isApprox(Eigen::Vector3f(-4, 0, 9)));
}

TEST(BaseTransform, NormalMatrixPreservesPerpendicularityUnderNonuniformScaleAndRotation) {
    Transform transform({1, 2, 3}, {2, 3, 4},
        Eigen::Quaternionf(Eigen::AngleAxisf(std::numbers::pi_v<float> / 4, Eigen::Vector3f::UnitZ())));
    const Eigen::Vector3f normal = transform.normalMatrix() * Eigen::Vector3f(1, 0, 1);
    const auto affine = transform.affine();

    EXPECT_NEAR(normal.dot(affine.linear() * Eigen::Vector3f(1, 0, -1)), 0.0f, 1e-5f);
    EXPECT_NEAR(normal.dot(affine.linear() * Eigen::Vector3f(0, 1, 0)), 0.0f, 1e-5f);

    const auto before = transform.normalMatrix();
    transform.translation() = {-4, 5, -6};
    EXPECT_TRUE(transform.normalMatrix().isApprox(before));
}

TEST(BaseTransform, RejectsSingularAndNonfiniteNormalMatrices) {
    Transform transform;
    transform.scale().z() = 0;
    EXPECT_NO_THROW(transform.affine());
    EXPECT_THROW(transform.normalMatrix(), std::invalid_argument);

    transform.scale().setConstant(std::numeric_limits<float>::denorm_min());
    EXPECT_THROW(transform.normalMatrix(), std::invalid_argument);
}

TEST(BaseTransform, RejectsNonfiniteAffineResults) {
    Transform transform;
    transform.translation().x() = std::numeric_limits<float>::quiet_NaN();
    EXPECT_THROW(transform.affine(), std::invalid_argument);
    EXPECT_THROW(transform.normalMatrix(), std::invalid_argument);

    transform.reset();
    transform.rotation().w() = std::numeric_limits<float>::infinity();
    EXPECT_THROW(transform.affine(), std::invalid_argument);

    transform.reset();
    transform.scale().x() = std::numeric_limits<float>::infinity();
    EXPECT_THROW(transform.affine(), std::invalid_argument);

    transform.reset();
    transform.translation().x() = std::numeric_limits<float>::max();
    transform.scale().x() = 2;
    EXPECT_THROW(transform.affine(), std::invalid_argument);

    transform.reset();
    EXPECT_TRUE(transform.affine().matrix().isIdentity());
}
