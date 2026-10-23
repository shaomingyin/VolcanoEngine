//
//
#ifndef VOLCANO_MATH_VECTOR_H
#define VOLCANO_MATH_VECTOR_H

#include <cassert>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <stdexcept>
#include <type_traits>

#include <Volcano/Math/Common.h>

VOLCANO_MATH_BEGIN

template <typename T, std::size_t N>
struct Vector;

template <typename T>
struct Vector<T, 2> {
    T x = {};
    T y = {};

    constexpr Vector() = default;

    constexpr explicit Vector(T value) : x(value), y(value) {
    }

    constexpr Vector(T x, T y) : x(x), y(y) {
    }

    template <typename U>
    constexpr explicit Vector(const Vector<U, 2> &v) : x(T(v.x)), y(T(v.y)) {
    }

    constexpr T &operator[](std::size_t i) noexcept {
        assert(i < 2);
        return i == 0 ? x : y;
    }

    constexpr const T &operator[](std::size_t i) const noexcept {
        assert(i < 2);
        return i == 0 ? x : y;
    }

    constexpr Vector &operator+=(const Vector &other) noexcept {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vector &operator-=(const Vector &other) noexcept {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Vector &operator*=(const Vector &other) noexcept {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    constexpr Vector &operator/=(const Vector &other) {
        if (other.x == T{} || other.y == T{}) {
            throw std::domain_error("Vector divisor components must be nonzero");
        }
        x /= other.x;
        y /= other.y;
        return *this;
    }

    constexpr Vector &operator*=(T scalar) noexcept {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    constexpr Vector &operator/=(T scalar) {
        if (scalar == T{}) {
            throw std::domain_error("Vector divisor must be nonzero");
        }
        x /= scalar;
        y /= scalar;
        return *this;
    }

    T length() const noexcept requires std::floating_point<T> {
        T result = {};
        for (std::size_t i = 0; i < 2; ++i) {
            result = std::hypot(result, (*this)[i]);
        }
        return result;
    }

    Vector normalized() const requires std::floating_point<T> {
        const T magnitude = length();
        if (!(magnitude > T{}) || !std::isfinite(magnitude)) {
            throw std::domain_error("Cannot normalize a zero or non-finite vector");
        }
        Vector result = *this;
        return result /= magnitude;
    }

    Vector &normalize() requires std::floating_point<T> {
        return *this = normalized();
    }

    constexpr T dot(const Vector &other) const noexcept {
        T result = {};
        for (std::size_t i = 0; i < 2; ++i) {
            result += (*this)[i] * other[i];
        }
        return result;
    }

    T distance(const Vector &other) const noexcept requires std::floating_point<T> {
        return (*this - other).length();
    }

    constexpr Vector mix(const Vector &other, T t) const noexcept requires std::floating_point<T> {
        return *this * (T(1) - t) + other * t;
    }

    constexpr bool operator==(const Vector &) const = default;
};

template <typename T>
struct Vector<T, 3> {
    T x = {};
    T y = {};
    T z = {};

    constexpr Vector() = default;

    constexpr explicit Vector(T value) : x(value), y(value), z(value) {
    }

    constexpr Vector(T x, T y, T z) : x(x), y(y), z(z) {
    }

    constexpr Vector(const Vector<T, 2> &v, T z) : x(v.x), y(v.y), z(z) {
    }

    template <typename U>
    constexpr explicit Vector(const Vector<U, 3> &v) : x(T(v.x)), y(T(v.y)), z(T(v.z)) {
    }

    constexpr T &operator[](std::size_t i) noexcept {
        assert(i < 3);
        return i == 0 ? x : (i == 1 ? y : z);
    }

    constexpr const T &operator[](std::size_t i) const noexcept {
        assert(i < 3);
        return i == 0 ? x : (i == 1 ? y : z);
    }

    constexpr Vector &operator+=(const Vector &other) noexcept {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr Vector &operator-=(const Vector &other) noexcept {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    constexpr Vector &operator*=(const Vector &other) noexcept {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    constexpr Vector &operator/=(const Vector &other) {
        if (other.x == T{} || other.y == T{} || other.z == T{}) {
            throw std::domain_error("Vector divisor components must be nonzero");
        }
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    constexpr Vector &operator*=(T scalar) noexcept {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    constexpr Vector &operator/=(T scalar) {
        if (scalar == T{}) {
            throw std::domain_error("Vector divisor must be nonzero");
        }
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    T length() const noexcept requires std::floating_point<T> {
        T result = {};
        for (std::size_t i = 0; i < 3; ++i) {
            result = std::hypot(result, (*this)[i]);
        }
        return result;
    }

    Vector normalized() const requires std::floating_point<T> {
        const T magnitude = length();
        if (!(magnitude > T{}) || !std::isfinite(magnitude)) {
            throw std::domain_error("Cannot normalize a zero or non-finite vector");
        }
        Vector result = *this;
        return result /= magnitude;
    }

    Vector &normalize() requires std::floating_point<T> {
        return *this = normalized();
    }

    constexpr T dot(const Vector &other) const noexcept {
        T result = {};
        for (std::size_t i = 0; i < 3; ++i) {
            result += (*this)[i] * other[i];
        }
        return result;
    }

    T distance(const Vector &other) const noexcept requires std::floating_point<T> {
        return (*this - other).length();
    }

    constexpr Vector mix(const Vector &other, T t) const noexcept requires std::floating_point<T> {
        return *this * (T(1) - t) + other * t;
    }

    constexpr Vector cross(const Vector &other) const noexcept {
        return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
    }

    constexpr bool operator==(const Vector &) const = default;
};

template <typename T>
struct Vector<T, 4> {
    T x = {};
    T y = {};
    T z = {};
    T w = {};

    constexpr Vector() = default;

    constexpr explicit Vector(T value) : x(value), y(value), z(value), w(value) {
    }

    constexpr Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {
    }

    constexpr Vector(const Vector<T, 3> &v, T w) : x(v.x), y(v.y), z(v.z), w(w) {
    }

    template <typename U>
    constexpr explicit Vector(const Vector<U, 4> &v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {
    }

    constexpr T &operator[](std::size_t i) noexcept {
        assert(i < 4);
        return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w));
    }

    constexpr const T &operator[](std::size_t i) const noexcept {
        assert(i < 4);
        return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w));
    }

    constexpr Vector &operator+=(const Vector &other) noexcept {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    constexpr Vector &operator-=(const Vector &other) noexcept {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    constexpr Vector &operator*=(const Vector &other) noexcept {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }

    constexpr Vector &operator/=(const Vector &other) {
        if (other.x == T{} || other.y == T{} || other.z == T{} || other.w == T{}) {
            throw std::domain_error("Vector divisor components must be nonzero");
        }
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
    }

    constexpr Vector &operator*=(T scalar) noexcept {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    constexpr Vector &operator/=(T scalar) {
        if (scalar == T{}) {
            throw std::domain_error("Vector divisor must be nonzero");
        }
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    T length() const noexcept requires std::floating_point<T> {
        T result = {};
        for (std::size_t i = 0; i < 4; ++i) {
            result = std::hypot(result, (*this)[i]);
        }
        return result;
    }

    Vector normalized() const requires std::floating_point<T> {
        const T magnitude = length();
        if (!(magnitude > T{}) || !std::isfinite(magnitude)) {
            throw std::domain_error("Cannot normalize a zero or non-finite vector");
        }
        Vector result = *this;
        return result /= magnitude;
    }

    Vector &normalize() requires std::floating_point<T> {
        return *this = normalized();
    }

    constexpr T dot(const Vector &other) const noexcept {
        T result = {};
        for (std::size_t i = 0; i < 4; ++i) {
            result += (*this)[i] * other[i];
        }
        return result;
    }

    T distance(const Vector &other) const noexcept requires std::floating_point<T> {
        return (*this - other).length();
    }

    constexpr Vector mix(const Vector &other, T t) const noexcept requires std::floating_point<T> {
        return *this * (T(1) - t) + other * t;
    }

    constexpr bool operator==(const Vector &) const = default;
};

template <typename T, std::size_t N>
constexpr Vector<T, N> operator+(Vector<T, N> a, const Vector<T, N> &b) noexcept {
    return a += b;
}

template <typename T, std::size_t N>
constexpr Vector<T, N> operator-(Vector<T, N> a, const Vector<T, N> &b) noexcept {
    return a -= b;
}

template <typename T, std::size_t N>
constexpr Vector<T, N> operator-(Vector<T, N> v) noexcept {
    for (std::size_t i = 0; i < N; ++i) {
        v[i] = -v[i];
    }
    return v;
}

template <typename T, std::size_t N>
constexpr Vector<T, N> operator*(Vector<T, N> a, const Vector<T, N> &b) noexcept {
    return a *= b;
}

template <typename T, std::size_t N>
constexpr Vector<T, N> operator/(Vector<T, N> a, const Vector<T, N> &b) {
    return a /= b;
}

template <typename T, std::size_t N>
constexpr Vector<T, N> operator*(Vector<T, N> v, std::type_identity_t<T> scalar) noexcept {
    return v *= scalar;
}

template <typename T, std::size_t N>
constexpr Vector<T, N> operator*(std::type_identity_t<T> scalar, Vector<T, N> v) noexcept {
    return v *= scalar;
}

template <typename T, std::size_t N>
constexpr Vector<T, N> operator/(Vector<T, N> v, std::type_identity_t<T> scalar) {
    return v /= scalar;
}

template <typename T> using Vector2 = Vector<T, 2>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;

template <typename T> using Vector3 = Vector<T, 3>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3i = Vector3<int>;
using Vector3u = Vector3<unsigned int>;

template <typename T> using Vector4 = Vector<T, 4>;
using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;
using Vector4i = Vector4<int>;
using Vector4u = Vector4<unsigned int>;

VOLCANO_MATH_END

#endif // VOLCANO_MATH_VECTOR_H
