//
//
#ifndef VOLCANO_MATH_QUATERNION_H
#define VOLCANO_MATH_QUATERNION_H

#include <algorithm>
#include <limits>

#include <Volcano/Math/Matrix.h>

#include <Volcano/Math/Common.h>

VOLCANO_MATH_BEGIN

template <std::floating_point T>
struct Quaternion {
    T w = T(1);
    T x = {};
    T y = {};
    T z = {};

    constexpr Quaternion() = default;

    constexpr Quaternion(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {
    }

    Quaternion normalized() const {
        const auto v = Vector4<T>(w, x, y, z).normalized();
        return {v.x, v.y, v.z, v.w};
    }

    Quaternion &normalize() {
        return *this = normalized();
    }

    constexpr Quaternion conjugate() const noexcept {
        return {w, -x, -y, -z};
    }

    Quaternion inverse() const {
        const T magnitude = Vector4<T>(w, x, y, z).length();
        if (!(magnitude > T{}) || !std::isfinite(magnitude)) {
            throw std::domain_error("Cannot invert a zero or non-finite quaternion");
        }
        return {w / magnitude / magnitude, -x / magnitude / magnitude,
                -y / magnitude / magnitude, -z / magnitude / magnitude};
    }

    Matrix3<T> toMatrix() const {
        const auto [w, x, y, z] = normalized();
        Matrix3<T> result;
        result[0][0] = T(1) - T(2) * (y * y + z * z);
        result[0][1] = T(2) * (x * y + w * z);
        result[0][2] = T(2) * (x * z - w * y);
        result[1][0] = T(2) * (x * y - w * z);
        result[1][1] = T(1) - T(2) * (x * x + z * z);
        result[1][2] = T(2) * (y * z + w * x);
        result[2][0] = T(2) * (x * z + w * y);
        result[2][1] = T(2) * (y * z - w * x);
        result[2][2] = T(1) - T(2) * (x * x + y * y);
        return result;
    }

    constexpr T dot(const Quaternion &other) const noexcept {
        return w * other.w + x * other.x + y * other.y + z * other.z;
    }

    static Quaternion from(T angle, const Vector3<T> &axis) {
        if (!std::isfinite(angle)) {
            throw std::invalid_argument("Rotation angle must be finite");
        }
        const auto direction = axis.normalized();
        const T halfAngle = angle / T(2);
        const auto v = direction * std::sin(halfAngle);
        return {std::cos(halfAngle), v.x, v.y, v.z};
    }

    Quaternion slerp(const Quaternion &to, T t) const {
        if (!std::isfinite(t)) {
            throw std::invalid_argument("Quaternion interpolation factor must be finite");
        }
        const auto a = normalized();
        auto b = to.normalized();
        T cosine = a.dot(b);
        if (cosine < T{}) {
            b = {-b.w, -b.x, -b.y, -b.z};
            cosine = -cosine;
        }
        cosine = std::clamp(cosine, T{}, T(1));
        T weightA = T(1) - t;
        T weightB = t;
        if (cosine < T(1) - std::numeric_limits<T>::epsilon()) {
            const T angle = std::acos(cosine);
            const T sine = std::sin(angle);
            weightA = std::sin((T(1) - t) * angle) / sine;
            weightB = std::sin(t * angle) / sine;
        }
        return Quaternion<T>(weightA * a.w + weightB * b.w,
            weightA * a.x + weightB * b.x, weightA * a.y + weightB * b.y,
            weightA * a.z + weightB * b.z).normalized();
    }

    // Input must be an orthonormal rotation matrix.
    static Quaternion from(const Matrix3<T> &m) {
        const T trace = m[0][0] + m[1][1] + m[2][2];
        Quaternion<T> result;
        if (trace > T{}) {
            const T divisor = T(2) * std::sqrt(trace + T(1));
            result = {divisor / T(4), (m[1][2] - m[2][1]) / divisor,
                      (m[2][0] - m[0][2]) / divisor, (m[0][1] - m[1][0]) / divisor};
        } else {
            std::size_t i = 0;
            if (m[1][1] > m[i][i]) {
                i = 1;
            }
            if (m[2][2] > m[i][i]) {
                i = 2;
            }
            const auto j = (i + 1) % 3;
            const auto k = (i + 2) % 3;
            const T divisor = T(2) * std::sqrt(T(1) + m[i][i] - m[j][j] - m[k][k]);
            Vector3<T> axis;
            axis[i] = divisor / T(4);
            axis[j] = (m[i][j] + m[j][i]) / divisor;
            axis[k] = (m[i][k] + m[k][i]) / divisor;
            result = {(m[j][k] - m[k][j]) / divisor, axis.x, axis.y, axis.z};
        }
        return result.normalized();
    }

    constexpr Quaternion &operator*=(const Quaternion &other) noexcept {
        return *this = Quaternion(w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w);
    }

    constexpr bool operator==(const Quaternion &) const = default;
};

template <std::floating_point T>
constexpr Quaternion<T> operator*(Quaternion<T> a, const Quaternion<T> &b) noexcept {
    return a *= b;
}

template <std::floating_point T>
Vector3<T> operator*(const Quaternion<T> &q, const Vector3<T> &v) {
    const auto unit = q.normalized();
    const Vector3<T> axis(unit.x, unit.y, unit.z);
    const auto twiceCross = T(2) * axis.cross(v);
    return v + unit.w * twiceCross + axis.cross(twiceCross);
}

template <std::floating_point T, std::size_t N>
Matrix<T, N> &Matrix<T, N>::rotate(const Quaternion<T> &q) requires (N == 4) {
    const auto rotation3 = q.toMatrix();
    Matrix rotation;
    for (std::size_t column = 0; column < 3; ++column) {
        for (std::size_t row = 0; row < 3; ++row) {
            rotation[column][row] = rotation3[column][row];
        }
    }
    return *this *= rotation;
}

using Quaternionf = Quaternion<float>;
using Quaterniond = Quaternion<double>;

VOLCANO_MATH_END

#endif // VOLCANO_MATH_QUATERNION_H
