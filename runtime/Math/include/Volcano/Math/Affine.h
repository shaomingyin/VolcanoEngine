//
//
#ifndef VOLCANO_MATH_AFFINE_H
#define VOLCANO_MATH_AFFINE_H

#include <Volcano/Math/Quaternion.h>
#include <Volcano/Math/Rect.h>
#include <Volcano/Math/BoundingBox.h>

#include <Volcano/Math/Common.h>

VOLCANO_MATH_BEGIN

template <std::floating_point T, std::size_t N>
class Affine {
    static_assert(N == 2 || N == 3);

public:
    Affine() = default;

    explicit Affine(const Matrix<T, N + 1> &matrix) : matrix_(matrix) {
        for (std::size_t column = 0; column <= N; ++column) {
            if (matrix_[column][N] != (column == N ? T(1) : T{})) {
                throw std::invalid_argument("Affine matrix must have last row [0, ..., 0, 1]");
            }
        }
    }

    const Matrix<T, N + 1> &matrix() const noexcept {
        return matrix_;
    }

    const T *data() const noexcept {
        return matrix_.data();
    }

    Affine &translate(const Vector<T, N> &offset) noexcept {
        matrix_.translate(offset);
        return *this;
    }

    Affine &rotate(T angle) requires (N == 2) {
        matrix_.rotate(angle);
        return *this;
    }

    Affine &rotate(const Quaternion<T> &rotation) requires (N == 3) {
        matrix_.rotate(rotation);
        return *this;
    }

    Affine &rotate(T angle, const Vector3<T> &axis) requires (N == 3) {
        matrix_.rotate(angle, axis);
        return *this;
    }

    Affine &scale(const Vector<T, N> &factors) noexcept {
        matrix_.scale(factors);
        return *this;
    }

    Vector<T, N> transform(const Vector<T, N> &point) const {
        return matrix_.transform(point);
    }

    Vector<T, N> transformVector(const Vector<T, N> &vector) const noexcept {
        return matrix_.transformVector(vector);
    }

    BasicRect<T> transform(const BasicRect<T> &rect) const requires (N == 2) {
        const std::array<Vector2<T>, 4> corners = {{
            {rect.left, rect.top}, {rect.left + rect.width, rect.top},
            {rect.left, rect.top + rect.height}, {rect.left + rect.width, rect.top + rect.height}}};
        const auto first = transform(corners[0]);
        T left = first.x, right = first.x, top = first.y, bottom = first.y;
        for (const auto &corner : corners) {
            const auto point = transform(corner);
            left = std::min(left, point.x);
            right = std::max(right, point.x);
            top = std::min(top, point.y);
            bottom = std::max(bottom, point.y);
        }
        return {left, top, right - left, bottom - top};
    }

    BoundingBox<T, N> transform(const BoundingBox<T, N>& box) const {
        BoundingBox<T, N> result;
        if (!box.empty()) {
            for (std::size_t corner = 0; corner < (std::size_t(1) << N); ++corner) {
                Vector<T, N> point;
                for (std::size_t i = 0; i < N; ++i) {
                    point[i] = (corner & (std::size_t(1) << i)) ? box.maximum()[i] : box.minimum()[i];
                }
                result.extend(transform(point));
            }
        }
        return result;
    }

    Affine inverse() const {
        auto matrix = matrix_.inverse();
        // The inverse is affine; restore its exact last row after floating-point elimination.
        for (std::size_t column = 0; column <= N; ++column) {
            matrix[column][N] = column == N ? T(1) : T{};
        }
        return Affine(matrix);
    }

    Affine &operator*=(const Affine &other) noexcept {
        matrix_ *= other.matrix_;
        return *this;
    }

private:
    Matrix<T, N + 1> matrix_;
};

template <std::floating_point T, std::size_t N>
Affine<T, N> operator*(Affine<T, N> a, const Affine<T, N> &b) noexcept {
    return a *= b;
}

template <std::floating_point T, std::size_t N>
Vector<T, N> operator*(const Affine<T, N> &transform, const Vector<T, N> &point) {
    return transform.transform(point);
}

template <std::floating_point T> using Affine2 = Affine<T, 2>;
template <std::floating_point T> using Affine3 = Affine<T, 3>;
using Affine2f = Affine2<float>;
using Affine2d = Affine2<double>;
using Affine3f = Affine3<float>;
using Affine3d = Affine3<double>;

VOLCANO_MATH_END

#endif // VOLCANO_MATH_AFFINE_H
