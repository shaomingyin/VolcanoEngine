#ifndef VOLCANO_MATH_H
#define VOLCANO_MATH_H

#include <concepts>
#include <numbers>
#include <stdexcept>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/LU>

#include <Volcano/Common.h>

namespace Volcano::Eigen {

// Keep third-party names visible when lookup finds this extension namespace first.
using namespace ::Eigen;

template <std::floating_point T>
constexpr T radians(T degrees) noexcept {
    return degrees * (std::numbers::pi_v<T> / T(180));
}

template <std::floating_point T>
constexpr T degrees(T radians) noexcept {
    return radians * (T(180) / std::numbers::pi_v<T>);
}

inline Eigen::Matrix3f normalMatrix(const Eigen::Affine3f& transform) {
    if (!transform.matrix().allFinite()) {
        throw std::invalid_argument("Transform must be finite");
    }

    const auto decomposition = transform.linear().fullPivLu();
    if (!decomposition.isInvertible()) {
        throw std::invalid_argument("Normal matrix requires an invertible transform");
    }

    const Eigen::Matrix3f normal = decomposition.inverse().transpose();
    if (!normal.allFinite()) {
        throw std::invalid_argument("Normal matrix must be finite");
    }

    return normal;
}

} // namespace Volcano::Eigen

#endif // VOLCANO_MATH_H
