//
//
#ifndef VOLCANO_MATH_MATRIX_H
#define VOLCANO_MATH_MATRIX_H

#include <array>
#include <numbers>
#include <utility>

#include <Volcano/Math/Vector.h>

#include <Volcano/Math/Common.h>

VOLCANO_MATH_BEGIN

template <std::floating_point T>
struct Quaternion;

template <std::floating_point T, std::size_t N>
class Matrix {
    static_assert(N >= 2 && N <= 4);

public:
    constexpr explicit Matrix(T diagonal = T(1)) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            (*this)[i][i] = diagonal;
        }
    }

    constexpr T *operator[](std::size_t column) noexcept {
        assert(column < N);
        return values_.data() + column * N;
    }

    constexpr const T *operator[](std::size_t column) const noexcept {
        assert(column < N);
        return values_.data() + column * N;
    }

    constexpr Matrix &operator*=(const Matrix &other) noexcept {
        Matrix result(T{});
        for (std::size_t column = 0; column < N; ++column) {
            for (std::size_t row = 0; row < N; ++row) {
                for (std::size_t k = 0; k < N; ++k) {
                    result[column][row] += (*this)[k][row] * other[column][k];
                }
            }
        }
        return *this = result;
    }

    constexpr Matrix &operator+=(const Matrix &other) noexcept {
        for (std::size_t i = 0; i < N * N; ++i) {
            values_[i] += other.values_[i];
        }
        return *this;
    }

    constexpr Matrix &operator-=(const Matrix &other) noexcept {
        for (std::size_t i = 0; i < N * N; ++i) {
            values_[i] -= other.values_[i];
        }
        return *this;
    }

    constexpr Matrix &operator*=(T scalar) noexcept {
        for (auto &value : values_) {
            value *= scalar;
        }
        return *this;
    }

    constexpr Matrix transposed() const noexcept {
        Matrix result(T{});
        for (std::size_t column = 0; column < N; ++column) {
            for (std::size_t row = 0; row < N; ++row) {
                result[column][row] = (*this)[row][column];
            }
        }
        return result;
    }

    T determinant() const noexcept {
        auto m = *this;
        T result = T(1);
        for (std::size_t column = 0; column < N; ++column) {
            const auto pivot = m.pivotRow(column);
            if (m[column][pivot] == T{}) {
                return T{};
            }
            if (pivot != column) {
                for (std::size_t k = 0; k < N; ++k) {
                    std::swap(m[k][pivot], m[k][column]);
                }
                result = -result;
            }
            result *= m[column][column];
            for (std::size_t row = column + 1; row < N; ++row) {
                const T factor = m[column][row] / m[column][column];
                for (std::size_t k = column + 1; k < N; ++k) {
                    m[k][row] -= factor * m[k][column];
                }
            }
        }
        return result;
    }

    Matrix inverse() const {
        auto m = *this;
        Matrix result;
        for (const T value : values_) {
            if (!std::isfinite(value)) {
                throw std::domain_error("Cannot invert a non-finite matrix");
            }
        }
        for (std::size_t column = 0; column < N; ++column) {
            const auto pivot = m.pivotRow(column);
            if (m[column][pivot] == T{}) {
                throw std::domain_error("Cannot invert a singular matrix");
            }
            for (std::size_t k = 0; k < N; ++k) {
                std::swap(m[k][pivot], m[k][column]);
                std::swap(result[k][pivot], result[k][column]);
            }
            const T divisor = m[column][column];
            for (std::size_t k = 0; k < N; ++k) {
                m[k][column] /= divisor;
                result[k][column] /= divisor;
            }
            for (std::size_t row = 0; row < N; ++row) {
                if (row == column) {
                    continue;
                }
                const T factor = m[column][row];
                for (std::size_t k = 0; k < N; ++k) {
                    m[k][row] -= factor * m[k][column];
                    result[k][row] -= factor * result[k][column];
                }
            }
        }
        return result;
    }

    Vector<T, N - 1> transform(const Vector<T, N - 1> &point) const requires (N == 3 || N == 4) {
        Vector<T, N> homogeneous;
        for (std::size_t i = 0; i < N - 1; ++i) {
            homogeneous[i] = point[i];
        }
        homogeneous[N - 1] = T(1);
        const auto result = *this * homogeneous;
        const T w = result[N - 1];
        if (w == T{} || !std::isfinite(w)) {
            throw std::domain_error("Transformed point has zero or non-finite homogeneous coordinate");
        }
        Vector<T, N - 1> output;
        for (std::size_t i = 0; i < N - 1; ++i) {
            output[i] = result[i] / w;
        }
        return output;
    }

    Vector<T, N - 1> transformVector(const Vector<T, N - 1> &vector) const noexcept requires (N == 3 || N == 4) {
        Vector<T, N> homogeneous;
        for (std::size_t i = 0; i < N - 1; ++i) {
            homogeneous[i] = vector[i];
        }
        const auto result = *this * homogeneous;
        Vector<T, N - 1> output;
        for (std::size_t i = 0; i < N - 1; ++i) {
            output[i] = result[i];
        }
        return output;
    }

    Matrix<T, 3> normalMatrix() const requires (N == 4) {
        Matrix<T, 3> linear;
        for (std::size_t column = 0; column < 3; ++column) {
            for (std::size_t row = 0; row < 3; ++row) {
                linear[column][row] = (*this)[column][row];
            }
        }
        return linear.inverse().transposed();
    }

    Matrix &translate(const Vector<T, N - 1> &offset) noexcept requires (N == 3 || N == 4) {
        Matrix translation;
        for (std::size_t i = 0; i < N - 1; ++i) {
            translation[N - 1][i] = offset[i];
        }
        return *this *= translation;
    }

    Matrix &scale(const Vector<T, N - 1> &factors) noexcept requires (N == 3 || N == 4) {
        Matrix scaling;
        for (std::size_t i = 0; i < N - 1; ++i) {
            scaling[i][i] = factors[i];
        }
        return *this *= scaling;
    }

    Matrix &rotate(T angle) requires (N == 3) {
        if (!std::isfinite(angle)) {
            throw std::invalid_argument("Rotation angle must be finite");
        }
        Matrix rotation;
        rotation[0][0] = rotation[1][1] = std::cos(angle);
        rotation[0][1] = std::sin(angle);
        rotation[1][0] = -rotation[0][1];
        return *this *= rotation;
    }

    Matrix &rotate(T angle, const Vector3<T> &axis) requires (N == 4) {
        if (!std::isfinite(angle)) {
            throw std::invalid_argument("Rotation angle must be finite");
        }
        const auto [x, y, z] = axis.normalized();
        const T c = std::cos(angle), s = std::sin(angle), t = T(1) - c;
        Matrix rotation;
        rotation[0][0] = c + x * x * t;
        rotation[0][1] = y * x * t + z * s;
        rotation[0][2] = z * x * t - y * s;
        rotation[1][0] = x * y * t - z * s;
        rotation[1][1] = c + y * y * t;
        rotation[1][2] = z * y * t + x * s;
        rotation[2][0] = x * z * t + y * s;
        rotation[2][1] = y * z * t - x * s;
        rotation[2][2] = c + z * z * t;
        return *this *= rotation;
    }

    Matrix &rotate(const Quaternion<T> &rotation) requires (N == 4);

    static Matrix lookAt(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) requires (N == 4) {
        const auto forward = (target - eye).normalized();
        const auto right = forward.cross(up.normalized()).normalized();
        const auto cameraUp = right.cross(forward);
        Matrix view;
        for (std::size_t column = 0; column < 3; ++column) {
            view[column][0] = right[column];
            view[column][1] = cameraUp[column];
            view[column][2] = -forward[column];
        }
        view[3][0] = -right.dot(eye);
        view[3][1] = -cameraUp.dot(eye);
        view[3][2] = forward.dot(eye);
        return view;
    }

    static Matrix perspective(T verticalFov, T aspect, T nearPlane, T farPlane) requires (N == 4) {
        if (!std::isfinite(verticalFov) || !std::isfinite(aspect) ||
            !std::isfinite(nearPlane) || !std::isfinite(farPlane) ||
            !(verticalFov > T{} && verticalFov < std::numbers::pi_v<T>) ||
            !(aspect > T{}) || !(nearPlane > T{} && farPlane > nearPlane)) {
            throw std::invalid_argument("Perspective requires 0 < fov < pi, aspect > 0 and 0 < near < far");
        }
        const T cotangent = T(1) / std::tan(verticalFov / T(2));
        const T depth = farPlane - nearPlane;
        Matrix projection(T{});
        projection[0][0] = cotangent / aspect;
        projection[1][1] = cotangent;
        projection[2][2] = -(farPlane + nearPlane) / depth;
        projection[2][3] = -T(1);
        projection[3][2] = -T(2) * nearPlane * farPlane / depth;
        return projection;
    }

    static Matrix ortho(T left, T right, T bottom, T top, T nearPlane = T(-1), T farPlane = T(1)) requires (N == 4) {
        if (!std::isfinite(left) || !std::isfinite(right) || !std::isfinite(bottom) ||
            !std::isfinite(top) || !std::isfinite(nearPlane) || !std::isfinite(farPlane) ||
            left == right || bottom == top || nearPlane == farPlane) {
            throw std::invalid_argument("Orthographic bounds must be finite with nonzero extents");
        }
        Matrix projection;
        projection[0][0] = T(2) / (right - left);
        projection[1][1] = T(2) / (top - bottom);
        projection[2][2] = T(-2) / (farPlane - nearPlane);
        projection[3][0] = -(right + left) / (right - left);
        projection[3][1] = -(top + bottom) / (top - bottom);
        projection[3][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        return projection;
    }

    constexpr T *data() noexcept {
        return values_.data();
    }

    constexpr const T *data() const noexcept {
        return values_.data();
    }

    constexpr bool operator==(const Matrix &) const = default;

private:
    std::size_t pivotRow(std::size_t column) const noexcept {
        std::size_t pivot = column;
        for (std::size_t row = column + 1; row < N; ++row) {
            if (std::abs((*this)[column][row]) > std::abs((*this)[column][pivot])) {
                pivot = row;
            }
        }
        return pivot;
    }

    std::array<T, N * N> values_ = {};
};

template <std::floating_point T, std::size_t N>
constexpr Vector<T, N> operator*(const Matrix<T, N> &m, const Vector<T, N> &v) noexcept {
    Vector<T, N> result;
    for (std::size_t row = 0; row < N; ++row) {
        for (std::size_t column = 0; column < N; ++column) {
            result[row] += m[column][row] * v[column];
        }
    }
    return result;
}

template <std::floating_point T, std::size_t N>
constexpr Matrix<T, N> operator*(Matrix<T, N> a, const Matrix<T, N> &b) noexcept {
    return a *= b;
}

template <std::floating_point T, std::size_t N>
constexpr Matrix<T, N> operator+(Matrix<T, N> a, const Matrix<T, N> &b) noexcept {
    return a += b;
}

template <std::floating_point T, std::size_t N>
constexpr Matrix<T, N> operator-(Matrix<T, N> a, const Matrix<T, N> &b) noexcept {
    return a -= b;
}

template <std::floating_point T, std::size_t N>
constexpr Matrix<T, N> operator*(Matrix<T, N> m, std::type_identity_t<T> scalar) noexcept {
    return m *= scalar;
}

template <std::floating_point T, std::size_t N>
constexpr Matrix<T, N> operator*(std::type_identity_t<T> scalar, Matrix<T, N> m) noexcept {
    return m * scalar;
}

template <std::floating_point T> using Matrix2 = Matrix<T, 2>;
template <std::floating_point T> using Matrix3 = Matrix<T, 3>;
template <std::floating_point T> using Matrix4 = Matrix<T, 4>;
using Matrix2f = Matrix2<float>;
using Matrix2d = Matrix2<double>;
using Matrix3f = Matrix3<float>;
using Matrix3d = Matrix3<double>;
using Matrix4f = Matrix4<float>;
using Matrix4d = Matrix4<double>;

VOLCANO_MATH_END

#endif // VOLCANO_MATH_MATRIX_H
