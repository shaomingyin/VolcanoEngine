//
//
#ifndef VOLCANO_MATH_H
#define VOLCANO_MATH_H

#include <numbers>

#include <Volcano/Math/Vector.h>
#include <Volcano/Math/Matrix.h>
#include <Volcano/Math/Quaternion.h>
#include <Volcano/Math/BoundingBox.h>
#include <Volcano/Math/Rect.h>
#include <Volcano/Math/Affine.h>
#include <Volcano/Math/Color.h>

#include <Volcano/Math/Common.h>

VOLCANO_MATH_BEGIN

template <std::floating_point T>
constexpr T radians(T degrees) noexcept {
    return degrees * (std::numbers::pi_v<T> / T(180));
}

template <std::floating_point T>
constexpr T degrees(T radians) noexcept {
    return radians * (T(180) / std::numbers::pi_v<T>);
}

VOLCANO_MATH_END

#endif // VOLCANO_MATH_H
