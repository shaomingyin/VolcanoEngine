//
//
#ifndef VOLCANO_MATH_RECT_H
#define VOLCANO_MATH_RECT_H

#include <algorithm>
#include <numeric>

#include <Volcano/Math/Vector.h>

#include <Volcano/Math/Common.h>

VOLCANO_MATH_BEGIN

// Geometric queries accept negative sizes and use half-open bounds for both integer and floating-point rectangles.
template <typename T>
struct BasicRect {
    T left = {};
    T top = {};
    T width = {};
    T height = {};

    constexpr Vector2<T> position() const noexcept {
        return {left, top};
    }

    constexpr Vector2<T> size() const noexcept {
        return {width, height};
    }

    constexpr T right() const noexcept {
        return left + width;
    }

    constexpr T bottom() const noexcept {
        return top + height;
    }

    constexpr Vector2<T> topLeft() const noexcept {
        return position();
    }

    constexpr Vector2<T> topRight() const noexcept {
        return {right(), top};
    }

    constexpr Vector2<T> bottomLeft() const noexcept {
        return {left, bottom()};
    }

    constexpr Vector2<T> bottomRight() const noexcept {
        return {right(), bottom()};
    }

    constexpr Vector2<T> center() const noexcept {
        return {std::midpoint(std::min(left, right()), std::max(left, right())),
                std::midpoint(std::min(top, bottom()), std::max(top, bottom()))};
    }

    constexpr bool empty() const noexcept {
        return width == T{} || height == T{};
    }

    constexpr bool null() const noexcept {
        return width == T{} && height == T{};
    }

    constexpr BasicRect normalized() const noexcept {
        const T x = std::min(left, right());
        const T y = std::min(top, bottom());
        return {x, y, std::max(left, right()) - x, std::max(top, bottom()) - y};
    }

    constexpr BasicRect& normalize() noexcept {
        return *this = normalized();
    }

    constexpr void setLeft(T x) noexcept {
        width = right() - x;
        left = x;
    }

    constexpr void setTop(T y) noexcept {
        height = bottom() - y;
        top = y;
    }

    constexpr void setRight(T x) noexcept {
        width = x - left;
    }

    constexpr void setBottom(T y) noexcept {
        height = y - top;
    }

    constexpr void setSize(const Vector2<T>& value) noexcept {
        width = value.x;
        height = value.y;
    }

    constexpr void moveTo(T x, T y) noexcept {
        left = x;
        top = y;
    }

    constexpr void moveTo(const Vector2<T>& point) noexcept {
        moveTo(point.x, point.y);
    }

    constexpr void moveCenter(const Vector2<T>& point) noexcept {
        translate(point - center());
    }

    constexpr BasicRect& translate(T dx, T dy) noexcept {
        left += dx;
        top += dy;
        return *this;
    }

    constexpr BasicRect& translate(const Vector2<T>& offset) noexcept {
        return translate(offset.x, offset.y);
    }

    constexpr BasicRect translated(T dx, T dy) const noexcept {
        auto result = *this;
        result.translate(dx, dy);
        return result;
    }

    constexpr BasicRect translated(const Vector2<T>& offset) const noexcept {
        return translated(offset.x, offset.y);
    }

    constexpr BasicRect& adjust(T dx1, T dy1, T dx2, T dy2) noexcept {
        left += dx1;
        top += dy1;
        width += dx2 - dx1;
        height += dy2 - dy1;
        return *this;
    }

    constexpr BasicRect adjusted(T dx1, T dy1, T dx2, T dy2) const noexcept {
        auto result = *this;
        result.adjust(dx1, dy1, dx2, dy2);
        return result;
    }

    constexpr BasicRect transposed() const noexcept {
        return {left, top, height, width};
    }

    constexpr bool contains(T x, T y) const noexcept {
        return x >= std::min(left, right()) && x < std::max(left, right()) &&
               y >= std::min(top, bottom()) && y < std::max(top, bottom());
    }

    constexpr bool contains(const Vector2<T>& point) const noexcept {
        return contains(point.x, point.y);
    }

    constexpr bool contains(const BasicRect& other) const noexcept {
        const auto a = normalized();
        const auto b = other.normalized();
        return !a.empty() && !b.empty() && b.left >= a.left && b.top >= a.top &&
               b.right() <= a.right() && b.bottom() <= a.bottom();
    }

    constexpr bool intersects(const BasicRect& other) const noexcept {
        const auto a = normalized();
        const auto b = other.normalized();
        return !a.empty() && !b.empty() && a.left < b.right() && b.left < a.right() &&
               a.top < b.bottom() && b.top < a.bottom();
    }

    constexpr BasicRect intersected(const BasicRect& other) const noexcept {
        if (!intersects(other)) {
            return {};
        }
        const auto a = normalized();
        const auto b = other.normalized();
        const T x = std::max(a.left, b.left);
        const T y = std::max(a.top, b.top);
        return {x, y, std::min(a.right(), b.right()) - x, std::min(a.bottom(), b.bottom()) - y};
    }

    constexpr BasicRect united(const BasicRect& other) const noexcept {
        const auto a = normalized();
        const auto b = other.normalized();
        if (a.empty()) {
            return b.empty() ? BasicRect{} : b;
        }
        if (b.empty()) {
            return a;
        }
        const T x = std::min(a.left, b.left);
        const T y = std::min(a.top, b.top);
        return {x, y, std::max(a.right(), b.right()) - x, std::max(a.bottom(), b.bottom()) - y};
    }

    constexpr bool operator==(const BasicRect&) const noexcept = default;
};

using Rect = BasicRect<int>;
using Rectf = BasicRect<float>;

VOLCANO_MATH_END

#endif // VOLCANO_MATH_RECT_H
