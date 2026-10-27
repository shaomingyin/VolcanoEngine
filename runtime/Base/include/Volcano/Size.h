#ifndef VOLCANO_SIZE_H
#define VOLCANO_SIZE_H

#include <Volcano/Common.h>

VOLCANO_BEGIN

// Match Rect: preserve negative dimensions; a zero dimension makes the size empty.
template <typename T>
struct BasicSize {
public:
    constexpr bool empty() const noexcept {
        return width == T{} || height == T{};
    }

    constexpr bool null() const noexcept {
        return width == T{} && height == T{};
    }

    constexpr BasicSize transposed() const noexcept {
        return {height, width};
    }

    constexpr bool operator==(const BasicSize&) const noexcept = default;

public:
    T width = {};
    T height = {};
};

using Size = BasicSize<int>;
using Sizef = BasicSize<float>;

VOLCANO_END

#endif // VOLCANO_SIZE_H
