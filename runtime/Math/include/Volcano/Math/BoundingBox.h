//
//
#ifndef VOLCANO_MATH_BOUNDINGBOX_H
#define VOLCANO_MATH_BOUNDINGBOX_H

#include <algorithm>
#include <limits>
#include <numeric>

#include <Volcano/Math/Vector.h>

#include <Volcano/Math/Common.h>

VOLCANO_MATH_BEGIN

template <std::floating_point T, std::size_t N>
class BoundingBox final {
    static_assert(N == 2 || N == 3);

public:
    constexpr BoundingBox() noexcept {
        reset();
    }

    BoundingBox(const Vector<T, N>& minimum, const Vector<T, N>& maximum)
        : minimum_(minimum), maximum_(maximum) {
        requireFinite(minimum);
        requireFinite(maximum);
    }

    constexpr const Vector<T, N>& minimum() const noexcept {
        return minimum_;
    }

    constexpr const Vector<T, N>& maximum() const noexcept {
        return maximum_;
    }

    constexpr Vector<T, N> center() const noexcept {
        Vector<T, N> result;
        if (!empty()) {
            for (std::size_t i = 0; i < N; ++i) {
                result[i] = std::midpoint(minimum_[i], maximum_[i]);
            }
        }
        return result;
    }

    constexpr Vector<T, N> extent() const noexcept {
        Vector<T, N> result;
        if (!empty()) {
            for (std::size_t i = 0; i < N; ++i) {
                result[i] = std::midpoint(-minimum_[i], maximum_[i]);
            }
        }
        return result;
    }

    constexpr bool empty() const noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            if (minimum_[i] > maximum_[i]) {
                return true;
            }
        }
        return false;
    }

    constexpr void reset() noexcept {
        minimum_ = Vector<T, N>(std::numeric_limits<T>::max());
        maximum_ = Vector<T, N>(std::numeric_limits<T>::lowest());
    }

    void extend(const Vector<T, N>& point) {
        requireFinite(point);
        // An inverted box is empty, regardless of its stored endpoints.
        if (empty()) {
            minimum_ = maximum_ = point;
            return;
        }
        for (std::size_t i = 0; i < N; ++i) {
            minimum_[i] = std::min(minimum_[i], point[i]);
            maximum_[i] = std::max(maximum_[i], point[i]);
        }
    }

    void extend(const BoundingBox& other) {
        if (!other.empty()) {
            extend(other.minimum_);
            extend(other.maximum_);
        }
    }

    constexpr bool contains(const Vector<T, N>& point) const noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            if (!(point[i] >= minimum_[i] && point[i] <= maximum_[i])) {
                return false;
            }
        }
        return true;
    }

    constexpr bool contains(const BoundingBox& other) const noexcept {
        return !other.empty() && contains(other.minimum_) && contains(other.maximum_);
    }

    constexpr bool intersects(const BoundingBox& other) const noexcept {
        if (empty() || other.empty()) {
            return false;
        }
        for (std::size_t i = 0; i < N; ++i) {
            if (maximum_[i] < other.minimum_[i] || minimum_[i] > other.maximum_[i]) {
                return false;
            }
        }
        return true;
    }

    T distance(const Vector<T, N>& point) const {
        requireFinite(point);
        if (empty()) {
            return std::numeric_limits<T>::infinity();
        }
        T result = {};
        for (std::size_t i = 0; i < N; ++i) {
            const T delta = point[i] - std::clamp(point[i], minimum_[i], maximum_[i]);
            result = std::hypot(result, delta);
        }
        return result;
    }

private:
    static void requireFinite(const Vector<T, N>& point) {
        for (std::size_t i = 0; i < N; ++i) {
            if (!std::isfinite(point[i])) {
                throw std::invalid_argument("BoundingBox coordinates must be finite");
            }
        }
    }

    Vector<T, N> minimum_;
    Vector<T, N> maximum_;
};

template <std::floating_point T> using BoundingBox2 = BoundingBox<T, 2>;
template <std::floating_point T> using BoundingBox3 = BoundingBox<T, 3>;
using BoundingBox2f = BoundingBox2<float>;
using BoundingBox2d = BoundingBox2<double>;
using BoundingBox3f = BoundingBox3<float>;
using BoundingBox3d = BoundingBox3<double>;

VOLCANO_MATH_END

#endif // VOLCANO_MATH_BOUNDINGBOX_H
