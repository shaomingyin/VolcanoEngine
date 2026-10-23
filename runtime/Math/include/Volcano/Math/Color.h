//
//
#ifndef VOLCANO_MATH_COLOR_H
#define VOLCANO_MATH_COLOR_H

#include <algorithm>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <stdexcept>

#include <Volcano/Math/Vector.h>

#include <Volcano/Math/Common.h>

VOLCANO_MATH_BEGIN

// Straight alpha; float conversions rescale channels without changing color space.
template <typename T>
class BasicColor final {
    static_assert(std::is_same_v<T, std::uint8_t> || std::is_same_v<T, float>);
    using Input = std::conditional_t<std::is_same_v<T, std::uint8_t>, int, float>;
    static constexpr T maximum = std::is_same_v<T, std::uint8_t> ? T(255) : T(1);

public:
    constexpr BasicColor() noexcept = default;

    constexpr BasicColor(Input red, Input green, Input blue, Input alpha = maximum)
        : red_(channel(red)), green_(channel(green)), blue_(channel(blue)), alpha_(channel(alpha)) {
    }

    constexpr Input red() const noexcept {
        return red_;
    }

    constexpr Input green() const noexcept {
        return green_;
    }

    constexpr Input blue() const noexcept {
        return blue_;
    }

    constexpr Input alpha() const noexcept {
        return alpha_;
    }

    constexpr void setRed(Input value) {
        red_ = channel(value);
    }

    constexpr void setGreen(Input value) {
        green_ = channel(value);
    }

    constexpr void setBlue(Input value) {
        blue_ = channel(value);
    }

    constexpr void setAlpha(Input value) {
        alpha_ = channel(value);
    }

    constexpr float redF() const noexcept {
        return red_ / float(maximum);
    }

    constexpr float greenF() const noexcept {
        return green_ / float(maximum);
    }

    constexpr float blueF() const noexcept {
        return blue_ / float(maximum);
    }

    constexpr float alphaF() const noexcept {
        return alpha_ / float(maximum);
    }

    constexpr Vector4f toVector() const noexcept {
        return {redF(), greenF(), blueF(), alphaF()};
    }

    static constexpr BasicColor fromRgbF(float red, float green, float blue, float alpha = 1.0f) {
        if constexpr (std::is_same_v<T, std::uint8_t>) {
            return {quantize(red), quantize(green), quantize(blue), quantize(alpha)};
        } else {
            return {red, green, blue, alpha};
        }
    }

    static constexpr BasicColor fromVector(const Vector4f& rgba) {
        return fromRgbF(rgba.x, rgba.y, rgba.z, rgba.w);
    }

    constexpr BasicColor<float> toFloat() const;

    constexpr BasicColor<std::uint8_t> toByte() const;

    // The packed integer is always 0xRRGGBBAA, independently of host byte order.
    constexpr std::uint32_t toRgba() const noexcept(std::is_same_v<T, std::uint8_t>) {
        if constexpr (std::is_same_v<T, std::uint8_t>) {
            return (std::uint32_t(red_) << 24) | (std::uint32_t(green_) << 16) |
                   (std::uint32_t(blue_) << 8) | std::uint32_t(alpha_);
        } else {
            return toByte().toRgba();
        }
    }

    static constexpr BasicColor fromRgba(std::uint32_t rgba) noexcept {
        return fromRgbF(float((rgba >> 24) & 255u) / 255.0f, float((rgba >> 16) & 255u) / 255.0f,
                        float((rgba >> 8) & 255u) / 255.0f, float(rgba & 255u) / 255.0f);
    }

    static constexpr BasicColor fromHsvF(float hue, float saturation, float value, float alpha = 1.0f) {
        requireUnit(hue);
        requireUnit(saturation);
        if constexpr (std::is_same_v<T, std::uint8_t>) {
            requireUnit(value);
            requireUnit(alpha);
        } else {
            channel(value);
            channel(alpha);
            if (value < 0.0f) {
                throw std::out_of_range("HSV value must be nonnegative");
            }
        }
        const float sector = (hue == 1.0f ? 0.0f : hue) * 6.0f;
        const int index = static_cast<int>(sector);
        const float fraction = sector - index;
        const float low = value * (1.0f - saturation);
        const float falling = value * (1.0f - saturation * fraction);
        const float rising = value * (1.0f - saturation * (1.0f - fraction));
        switch (index) {
        case 0: return fromRgbF(value, rising, low, alpha);
        case 1: return fromRgbF(falling, value, low, alpha);
        case 2: return fromRgbF(low, value, rising, alpha);
        case 3: return fromRgbF(low, falling, value, alpha);
        case 4: return fromRgbF(rising, low, value, alpha);
        default: return fromRgbF(value, low, falling, alpha);
        }
    }

    constexpr Vector3f toHsvF() const noexcept(std::is_same_v<T, std::uint8_t>) {
        const float red = redF(), green = greenF(), blue = blueF();
        if constexpr (std::is_same_v<T, float>) {
            channel(red);
            channel(green);
            channel(blue);
            if (std::min({red, green, blue}) < 0.0f) {
                throw std::out_of_range("HSV conversion requires nonnegative RGB channels");
            }
        }
        const float maximum = std::max({red, green, blue});
        const float delta = maximum - std::min({red, green, blue});
        float hue = 0.0f;
        if (delta != 0.0f) {
            if (maximum == red) {
                hue = (green - blue) / delta;
            } else if (maximum == green) {
                hue = 2.0f + (blue - red) / delta;
            } else {
                hue = 4.0f + (red - green) / delta;
            }
            if (hue < 0.0f) {
                hue += 6.0f;
            }
            hue /= 6.0f;
        }
        return {hue, maximum == 0.0f ? 0.0f : delta / maximum, maximum};
    }

    // Arithmetic includes alpha and operates on stored channels, without alpha compositing.
    constexpr BasicColor& operator+=(const BasicColor& other) noexcept {
        red_ = add(red_, other.red_);
        green_ = add(green_, other.green_);
        blue_ = add(blue_, other.blue_);
        alpha_ = add(alpha_, other.alpha_);
        return *this;
    }

    constexpr BasicColor& operator-=(const BasicColor& other) noexcept {
        red_ = subtract(red_, other.red_);
        green_ = subtract(green_, other.green_);
        blue_ = subtract(blue_, other.blue_);
        alpha_ = subtract(alpha_, other.alpha_);
        return *this;
    }

    constexpr BasicColor& operator*=(const BasicColor& other) noexcept {
        red_ = multiply(red_, other.red_);
        green_ = multiply(green_, other.green_);
        blue_ = multiply(blue_, other.blue_);
        alpha_ = multiply(alpha_, other.alpha_);
        return *this;
    }

    constexpr bool operator==(const BasicColor&) const noexcept = default;

    inline static constexpr BasicColor Black{0, 0, 0};
    inline static constexpr BasicColor White{maximum, maximum, maximum};
    inline static constexpr BasicColor Red{maximum, 0, 0};
    inline static constexpr BasicColor Green{0, maximum, 0};
    inline static constexpr BasicColor Blue{0, 0, maximum};
    inline static constexpr BasicColor Yellow{maximum, maximum, 0};
    inline static constexpr BasicColor Magenta{maximum, 0, maximum};
    inline static constexpr BasicColor Cyan{0, maximum, maximum};
    inline static constexpr BasicColor Transparent{0, 0, 0, 0};

private:
    static constexpr T channel(Input value) {
        if constexpr (std::is_same_v<T, std::uint8_t>) {
            if (value < 0 || value > 255) {
                throw std::out_of_range("Color channel must be in [0, 255]");
            }
        } else {
            if (!(value >= std::numeric_limits<float>::lowest() && value <= std::numeric_limits<float>::max())) {
                throw std::out_of_range("Color channel must be finite");
            }
        }
        return static_cast<T>(value);
    }

    static constexpr T add(T left, T right) noexcept {
        if constexpr (std::is_same_v<T, std::uint8_t>) {
            return static_cast<T>(std::min(255, left + right));
        } else {
            return left + right;
        }
    }

    static constexpr T subtract(T left, T right) noexcept {
        if constexpr (std::is_same_v<T, std::uint8_t>) {
            return static_cast<T>(std::max(0, left - right));
        } else {
            return left - right;
        }
    }

    static constexpr T multiply(T left, T right) noexcept {
        if constexpr (std::is_same_v<T, std::uint8_t>) {
            return static_cast<T>(left * right / 255);
        } else {
            return left * right;
        }
    }

    static constexpr void requireUnit(float value) {
        if (!(value >= 0.0f && value <= 1.0f)) {
            throw std::out_of_range("BasicColor component must be finite and in [0, 1]");
        }
    }

    static constexpr int quantize(float value) {
        requireUnit(value);
        return static_cast<int>(value * 255.0f + 0.5f);
    }

    T red_ = 0;
    T green_ = 0;
    T blue_ = 0;
    T alpha_ = maximum;
};

template <typename T>
constexpr BasicColor<float> BasicColor<T>::toFloat() const {
    if constexpr (std::is_same_v<T, float>) {
        return *this;
    } else {
        return {redF(), greenF(), blueF(), alphaF()};
    }
}

template <typename T>
constexpr BasicColor<std::uint8_t> BasicColor<T>::toByte() const {
    if constexpr (std::is_same_v<T, std::uint8_t>) {
        return *this;
    } else {
        return BasicColor<std::uint8_t>::fromRgbF(std::clamp(red_, 0.0f, 1.0f), std::clamp(green_, 0.0f, 1.0f),
                                                 std::clamp(blue_, 0.0f, 1.0f), std::clamp(alpha_, 0.0f, 1.0f));
    }
}

template <typename T>
constexpr BasicColor<T> operator+(BasicColor<T> left, const BasicColor<T>& right) noexcept {
    return left += right;
}

template <typename T>
constexpr BasicColor<T> operator-(BasicColor<T> left, const BasicColor<T>& right) noexcept {
    return left -= right;
}

template <typename T>
constexpr BasicColor<T> operator*(BasicColor<T> left, const BasicColor<T>& right) noexcept {
    return left *= right;
}

using Color = BasicColor<std::uint8_t>;
using Colorf = BasicColor<float>;

VOLCANO_MATH_END

#endif // VOLCANO_MATH_COLOR_H
