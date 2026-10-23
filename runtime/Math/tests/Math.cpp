#include <gtest/gtest.h>
#include <limits>
#include <random>
#include <type_traits>

#include <Volcano/Math/Math.h>

namespace {

using namespace Volcano::Math;

template <typename T>
bool near(T a, T b) {
    const T epsilon = std::is_same_v<T, float> ? T(2e-5) : T(1e-12);
    return std::abs(a - b) <= epsilon * std::max(T(1), std::max(std::abs(a), std::abs(b)));
}

template <typename T, std::size_t N>
bool near(const Vector<T, N> &a, const Vector<T, N> &b) {
    for (std::size_t i = 0; i < N; ++i) {
        if (!near(a[i], b[i])) {
            return false;
        }
    }
    return true;
}

template <typename T, std::size_t N>
bool near(const Matrix<T, N> &a, const Matrix<T, N> &b) {
    for (std::size_t i = 0; i < N * N; ++i) {
        if (!near(a.data()[i], b.data()[i])) {
            return false;
        }
    }
    return true;
}

template <typename Exception, typename F>
void expectThrows(F operation) {
    EXPECT_THROW(operation(), Exception);
}

template <typename T, std::size_t N>
void checkVectorAssignments() {
    using V = Vector<T, N>;
    V v(T(12));
    const V other(T(3));
    EXPECT_TRUE((&v.operator+=(other) == &v && v == V(T(15))));
    EXPECT_TRUE((&v.operator-=(other) == &v && v == V(T(12))));
    EXPECT_TRUE((&v.operator*=(other) == &v && v == V(T(36))));
    EXPECT_TRUE((&v.operator/=(other) == &v && v == V(T(12))));
    EXPECT_TRUE((&v.operator*=(T(2)) == &v && v == V(T(24))));
    EXPECT_TRUE((&v.operator/=(T(2)) == &v && v == V(T(12))));
    auto divisor = other;
    divisor[N - 1] = T{};
    expectThrows<std::domain_error>([&] {
        v /= divisor;
    });
    EXPECT_TRUE((v == V(T(12))));
    expectThrows<std::domain_error>([&] {
        v /= T{};
    });
    EXPECT_TRUE((v == V(T(12))));
    v += v;
    v *= v;
    EXPECT_TRUE((v == V(T(576))));
    v /= v;
    EXPECT_TRUE((v == V(T(1))));
    v -= v;
    EXPECT_TRUE((v == V()));
}

template <typename T, std::size_t N>
void checkVectorNormalization() {
    using V = Vector<T, N>;
    const V original(T(2));
    const auto unit = original.normalized();
    EXPECT_TRUE((original == V(T(2))));
    EXPECT_TRUE((near(original.length(), T(2) * std::sqrt(T(N)))));
    EXPECT_TRUE((near(unit.length(), T(1))));
    EXPECT_TRUE((near(unit.dot(unit), T(1))));
    EXPECT_TRUE((near(original.distance(V()), original.length())));
    EXPECT_TRUE((near(original.mix(V(), T(0.25)), V(T(1.5)))));
    auto v = original;
    EXPECT_TRUE((&v.normalize() == &v && near(v, unit)));
    v = {};
    expectThrows<std::domain_error>([&] {
        v.normalize();
    });
    EXPECT_TRUE((v == V()));
    v[N - 1] = std::numeric_limits<T>::infinity();
    const auto invalid = v;
    expectThrows<std::domain_error>([&] {
        v.normalize();
    });
    EXPECT_TRUE((v == invalid));
    v[N - 1] = std::numeric_limits<T>::quiet_NaN();
    expectThrows<std::domain_error>([&] {
        v.normalize();
    });
    EXPECT_TRUE((std::isnan(v[N - 1]) && v[0] == T{}));
    EXPECT_TRUE((near(V(T(1e-30)).normalized().length(), T(1))));
}

template <typename T, std::size_t N>
void checkMatrix() {
    using M = Matrix<T, N>;
    M m;
    m[0][0] = T(0); m[0][1] = T(1);
    m[1][0] = T(2); m[1][1] = T(3);
    const M original = m;
    EXPECT_TRUE((near(original.determinant(), T(-2))));
    EXPECT_TRUE((near(original * original.inverse(), M())));
    EXPECT_TRUE((near(original.inverse() * original, M())));
    EXPECT_TRUE((near(original.transposed().transposed(), original)));
    EXPECT_TRUE((original == m));
    EXPECT_TRUE((m.data()[N] == T(2)));
    EXPECT_TRUE((&m.operator*=(m) == &m && near(m, original * original)));
    EXPECT_TRUE((near(m + m - m, m)));
    EXPECT_TRUE((near(T(2) * m, m * T(2))));
    const M tiny(T(1e-20));
    EXPECT_TRUE((near(tiny * tiny.inverse(), M())));
    expectThrows<std::domain_error>([] {
        M(T(0)).inverse();
    });
    M dependent;
    for (std::size_t row = 0; row < N; ++row) {
        dependent[1][row] = dependent[0][row];
    }
    expectThrows<std::domain_error>([&] {
        dependent.inverse();
    });
    m[0][0] = std::numeric_limits<T>::infinity();
    expectThrows<std::domain_error>([&] {
        m.inverse();
    });

    std::mt19937 generator(72419);
    std::uniform_real_distribution<T> random(T(-1), T(1));
    for (int iteration = 0; iteration < 50; ++iteration) {
        M general;
        for (std::size_t c = 0; c < N; ++c) {
            for (std::size_t r = 0; r < N; ++r) {
                general[c][r] = random(generator) + (c == r ? T(4) : T{});
            }
        }
        EXPECT_TRUE((near(general * general.inverse(), M())));
        EXPECT_TRUE((near(general.inverse() * general, M())));
        EXPECT_TRUE((near(general.determinant() * general.inverse().determinant(), T(1))));
    }
}

template <typename T>
void checkAffine() {
    const auto check = []<std::size_t N>() {
        using A = Affine<T, N>;
        using M = Matrix<T, N + 1>;
        EXPECT_TRUE((A().matrix() == M()));
        for (std::size_t column = 0; column <= N; ++column) {
            M invalid;
            invalid[column][N] = column == N ? T(2) : std::numeric_limits<T>::epsilon();
            expectThrows<std::invalid_argument>([&] { A affine(invalid); });
        }
        M nonfinite;
        nonfinite[0][N] = std::numeric_limits<T>::quiet_NaN();
        expectThrows<std::invalid_argument>([&] { A affine(nonfinite); });
        A singular;
        singular.scale(Vector<T, N>());
        expectThrows<std::domain_error>([&] { singular.inverse(); });

        std::mt19937 generator(72419);
        std::uniform_real_distribution<T> random(T(-1), T(1));
        for (int iteration = 0; iteration < 50; ++iteration) {
            M matrix;
            for (std::size_t column = 0; column <= N; ++column) {
                for (std::size_t row = 0; row < N; ++row) {
                    matrix[column][row] = random(generator) + (column == row ? T(4) : T{});
                }
            }
            const A affine(matrix);
            const auto inverse = affine.inverse();
            EXPECT_TRUE((near((affine * inverse).matrix(), M())));
            EXPECT_TRUE((near((inverse * affine).matrix(), M())));
            Vector<T, N> point(T(2));
            EXPECT_TRUE((near(inverse.transform(affine.transform(point)), point)));
            EXPECT_TRUE((near(affine.transform(point), affine.transformVector(point) + matrix.transform(Vector<T, N>()))));
            for (std::size_t column = 0; column <= N; ++column) {
                EXPECT_TRUE((inverse.matrix()[column][N] == (column == N ? T(1) : T{})));
            }
            auto self = affine;
            self *= self;
            EXPECT_TRUE((near(self.matrix(), matrix * matrix)));
        }
    };
    check.template operator()<2>();
    check.template operator()<3>();
}

template <typename T>
void checkGeometry() {
    using Q = Quaternion<T>;
    using M = Matrix4<T>;
    const T halfPi = std::numbers::pi_v<T> / T(2);
    const Vector3<T> x(1, 0, 0), y(0, 1, 0), z(0, 0, 1);
    EXPECT_TRUE((near(degrees(radians(T(180))), T(180))));
    EXPECT_TRUE((x.cross(y) == z && x.dot(y) == T{}));
    EXPECT_TRUE((near(Vector3<T>(3, 0, 4).length(), T(5))));
    EXPECT_TRUE((near(Vector3<T>(3, 0, 4).normalized(), Vector3<T>(T(0.6), 0, T(0.8)))));
    const auto rotation = Q::from(halfPi, z * T(2));
    EXPECT_TRUE((near(rotation * x, y)));
    EXPECT_TRUE((near(rotation.inverse() * y, x)));
    EXPECT_TRUE((near(rotation.toMatrix() * x, y)));
    EXPECT_TRUE((near(Q().slerp(rotation, T(0.5)) * x, Vector3<T>(std::sqrt(T(0.5)), std::sqrt(T(0.5)), 0))));
    const Q opposite(-rotation.w, -rotation.x, -rotation.y, -rotation.z);
    EXPECT_TRUE((near(rotation.slerp(opposite, T(0.5)) * x, y)));
    const auto small = Q::from(T(0.01), z);
    EXPECT_TRUE((near(Q().slerp(small, T(0.3)) * x, Q::from(T(0.003), z) * x)));
    const Q nonunit(2, 1, 3, 4);
    const auto unit = nonunit.normalized();
    auto normalized = nonunit;
    EXPECT_TRUE((&normalized.normalize() == &normalized && near(normalized.dot(unit), T(1))));
    EXPECT_TRUE((nonunit == Q(2, 1, 3, 4)));
    EXPECT_TRUE((nonunit.conjugate() == Q(2, -1, -3, -4)));
    EXPECT_TRUE((nonunit.conjugate().conjugate() == nonunit));
    EXPECT_TRUE((near((nonunit * nonunit.inverse()).dot(Q()), T(1))));
    EXPECT_TRUE((near(unit.toMatrix(), nonunit.toMatrix())));
    auto self = nonunit;
    EXPECT_TRUE((&self.operator*=(self) == &self && self == nonunit * nonunit));
    for (const auto &axis : {x, y, z, Vector3<T>(1, 2, 3).normalized()}) {
        const auto q = Q::from(std::numbers::pi_v<T>, axis);
        EXPECT_TRUE((near(Q::from(q.toMatrix()).toMatrix(), q.toMatrix())));
    }
    Q zero(0, 0, 0, 0);
    expectThrows<std::domain_error>([&] {
        zero.normalize();
    });
    EXPECT_TRUE((zero == Q(0, 0, 0, 0)));
    expectThrows<std::domain_error>([&] {
        zero.inverse();
    });
    Q invalid(std::numeric_limits<T>::infinity(), 1, 2, 3);
    const auto originalInvalid = invalid;
    expectThrows<std::domain_error>([&] {
        invalid.normalize();
    });
    EXPECT_TRUE((invalid == originalInvalid));
    expectThrows<std::domain_error>([&] {
        Q::from(halfPi, Vector3<T>());
    });
    expectThrows<std::invalid_argument>([&] {
        Q::from(std::numeric_limits<T>::infinity(), z);
    });
    expectThrows<std::invalid_argument>([&] {
        rotation.slerp(rotation, std::numeric_limits<T>::quiet_NaN());
    });

    Affine2<T> transform2;
    EXPECT_TRUE((&transform2.translate({3, 4}).rotate(halfPi).scale({2, 3}) == &transform2));
    const Vector2<T> point2(1, 0);
    EXPECT_TRUE((near(transform2 * point2, Vector2<T>(3, 6))));
    EXPECT_TRUE((near(transform2.transform(point2), Vector2<T>(3, 6))));
    EXPECT_TRUE((near(transform2.transformVector(point2), Vector2<T>(0, 2))));
    EXPECT_TRUE((near(transform2.inverse() * (transform2 * point2), point2)));
    EXPECT_TRUE((near((transform2 * transform2.inverse()).matrix(), Matrix3<T>())));
    Matrix3<T> matrix2;
    EXPECT_TRUE((&matrix2.translate({3, 4}).rotate(halfPi).scale({2, 3}) == &matrix2));
    EXPECT_TRUE((near(matrix2, transform2.matrix())));
    const auto bounds = transform2.transform({0, 0, 2, 1});
    EXPECT_TRUE((near(bounds.left, T(0)) && near(bounds.top, T(4))));
    EXPECT_TRUE((near(bounds.width, T(3)) && near(bounds.height, T(4))));
    const BasicRect<T> negative{2, 3, -2, -3};
    EXPECT_TRUE((negative.position() == Vector2<T>(2, 3) && negative.size() == Vector2<T>(-2, -3)));
    EXPECT_TRUE((negative.contains(T(0), T(0)) && !negative.contains(T(2), T(3))));
    Matrix3<T> crossing;
    crossing[0][2] = T(1); crossing[2][2] = T(0);
    expectThrows<std::invalid_argument>([&] {
        Affine2<T> invalid(crossing);
    });

    Affine3<T> transform3;
    EXPECT_TRUE((&transform3.translate({3, 4, 5}).rotate(rotation).scale({2, 3, 4}) == &transform3));
    EXPECT_TRUE((near(transform3 * x, Vector3<T>(3, 6, 5))));
    EXPECT_TRUE((near(transform3.transformVector(x), Vector3<T>(0, 2, 0))));
    EXPECT_TRUE((near(transform3.inverse() * (transform3 * x), x)));
    EXPECT_TRUE((near((transform3 * transform3.inverse()).matrix(), M())));
    M axisRotation;
    EXPECT_TRUE((&axisRotation.rotate(halfPi, z) == &axisRotation));
    EXPECT_TRUE((near(axisRotation.transformVector(x), y)));
    Affine3<T> axisTransform;
    EXPECT_TRUE((near(axisTransform.rotate(halfPi, z).matrix(), axisRotation)));
    M quaternionRotation;
    EXPECT_TRUE((&quaternionRotation.rotate(rotation) == &quaternionRotation));
    EXPECT_TRUE((near(quaternionRotation, axisRotation)));
    const auto normal = (transform3.matrix().normalMatrix() * Vector3<T>(1, 1, 0).normalized()).normalized();
    EXPECT_TRUE((near(normal.dot(transform3.transformVector(Vector3<T>(1, -1, 0))), T(0))));
    const auto before = quaternionRotation;
    expectThrows<std::invalid_argument>([&] {
        quaternionRotation.rotate(std::numeric_limits<T>::infinity(), z);
    });
    expectThrows<std::domain_error>([&] {
        quaternionRotation.rotate(T(1), Vector3<T>());
    });
    expectThrows<std::domain_error>([&] {
        quaternionRotation.rotate(zero);
    });
    EXPECT_TRUE((before == quaternionRotation));
    expectThrows<std::domain_error>([] {
        M(T(0)).normalMatrix();
    });

    const Vector3<T> eye(0, 0, 5), target;
    const auto view = M::lookAt(eye, target, y);
    EXPECT_TRUE((near(view.transform(eye), Vector3<T>())));
    EXPECT_TRUE((near(view.transform(target), Vector3<T>(0, 0, -5))));
    expectThrows<std::domain_error>([&] {
        M::lookAt(eye, eye, y);
    });
    expectThrows<std::domain_error>([&] {
        M::lookAt(eye, target, z);
    });
    const auto projection = M::perspective(halfPi, T(2), T(1), T(10));
    expectThrows<std::invalid_argument>([&] { Affine3<T> invalid(projection); });
    EXPECT_TRUE((near(projection.transform(Vector3<T>(0, 0, -1)).z, T(-1))));
    EXPECT_TRUE((near(projection.transform(Vector3<T>(0, 0, -10)).z, T(1))));
    EXPECT_TRUE((near(projection.transform(Vector3<T>(2, 1, -1)), Vector3<T>(1, 1, -1))));
    const auto combined = projection * view * transform3.matrix();
    const Vector3<T> point(1, 2, -3);
    EXPECT_TRUE((near(combined.inverse().transform(combined.transform(point)), point)));
    const auto orthographic = M::ortho(T(-2), T(2), T(-1), T(1), T(1), T(10));
    EXPECT_TRUE((Affine3<T>(orthographic).matrix() == orthographic));
    EXPECT_TRUE((near(orthographic.transform(Vector3<T>(-2, -1, -1)), Vector3<T>(-1, -1, -1))));
    EXPECT_TRUE((near(orthographic.transform(Vector3<T>(2, 1, -10)), Vector3<T>(1, 1, 1))));
    expectThrows<std::invalid_argument>([&] {
        M::perspective(T(0), T(1), T(1), T(10));
    });
    expectThrows<std::invalid_argument>([&] {
        M::perspective(halfPi, T(0), T(1), T(10));
    });
    expectThrows<std::invalid_argument>([&] {
        M::perspective(halfPi, T(1), T(-1), T(10));
    });
    expectThrows<std::invalid_argument>([&] {
        M::perspective(halfPi, T(1), T(10), T(1));
    });
    expectThrows<std::invalid_argument>([&] {
        M::perspective(std::numeric_limits<T>::quiet_NaN(), T(1), T(1), T(10));
    });
    expectThrows<std::invalid_argument>([&] {
        M::ortho(T(0), T(0), T(-1), T(1));
    });
    expectThrows<std::domain_error>([&] {
        projection.transform(Vector3<T>());
    });
}

} // namespace

TEST(Math, Checks) {
    static_assert(Vector2i(2, 3) + Vector2i(4, 5) == Vector2i(6, 8));
    static_assert(Vector4f() == Vector4f(0));
    static_assert(Matrix4f().data()[0] == 1);
    static_assert(Vector3i(1, 0, 0).cross(Vector3i(0, 1, 0)) == Vector3i(0, 0, 1));
    static_assert(std::is_same_v<Affine3f, Affine<float, 3>>);
    EXPECT_TRUE((Vector2u(640, 480).x == 640));
    EXPECT_TRUE((Vector3f(Vector3i(1, 2, 3)) == Vector3f(1, 2, 3)));
    EXPECT_TRUE((Vector2f(2, 3) * Vector2f(3, 4) == Vector2f(6, 12)));
    EXPECT_TRUE((Vector2f(6, 12) / Vector2f(3, 4) == Vector2f(2, 3)));
    EXPECT_TRUE((Vector4f(Vector3f(1, 2, 3), 4).w == 4));
}

TEST(Math, VectorAssignmentsInt2) {
    checkVectorAssignments<int, 2>();
}

TEST(Math, VectorAssignmentsInt3) {
    checkVectorAssignments<int, 3>();
}

TEST(Math, VectorAssignmentsInt4) {
    checkVectorAssignments<int, 4>();
}

TEST(Math, VectorAssignmentsFloat2) {
    checkVectorAssignments<float, 2>();
}

TEST(Math, VectorAssignmentsFloat3) {
    checkVectorAssignments<float, 3>();
}

TEST(Math, VectorAssignmentsFloat4) {
    checkVectorAssignments<float, 4>();
}

TEST(Math, VectorAssignmentsDouble2) {
    checkVectorAssignments<double, 2>();
}

TEST(Math, VectorAssignmentsDouble3) {
    checkVectorAssignments<double, 3>();
}

TEST(Math, VectorAssignmentsDouble4) {
    checkVectorAssignments<double, 4>();
}

TEST(Math, VectorNormalizationFloat2) {
    checkVectorNormalization<float, 2>();
}

TEST(Math, VectorNormalizationFloat3) {
    checkVectorNormalization<float, 3>();
}

TEST(Math, VectorNormalizationFloat4) {
    checkVectorNormalization<float, 4>();
}

TEST(Math, VectorNormalizationDouble2) {
    checkVectorNormalization<double, 2>();
}

TEST(Math, VectorNormalizationDouble3) {
    checkVectorNormalization<double, 3>();
}

TEST(Math, VectorNormalizationDouble4) {
    checkVectorNormalization<double, 4>();
}

TEST(Math, MatrixFloat2) {
    checkMatrix<float, 2>();
}

TEST(Math, MatrixFloat3) {
    checkMatrix<float, 3>();
}

TEST(Math, MatrixFloat4) {
    checkMatrix<float, 4>();
}

TEST(Math, MatrixDouble2) {
    checkMatrix<double, 2>();
}

TEST(Math, MatrixDouble3) {
    checkMatrix<double, 3>();
}

TEST(Math, MatrixDouble4) {
    checkMatrix<double, 4>();
}

TEST(Math, AffineFloat) {
    checkAffine<float>();
}

TEST(Math, AffineDouble) {
    checkAffine<double>();
}

TEST(Math, GeometryFloat) {
    checkGeometry<float>();
}

TEST(Math, GeometryDouble) {
    checkGeometry<double>();
}
