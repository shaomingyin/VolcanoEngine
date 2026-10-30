#ifndef VOLCANO_GUI_STYLE_H
#define VOLCANO_GUI_STYLE_H

#include <cmath>
#include <stdexcept>

#include <Volcano/Color.h>
#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

class Style final {
public:
    constexpr const Colorf& textColor() const noexcept {
        return text_color_;
    }
    constexpr void setTextColor(const Colorf& value) noexcept {
        text_color_ = value;
    }
    constexpr const Colorf& buttonPressedColor() const noexcept {
        return button_pressed_color_;
    }
    constexpr void setButtonPressedColor(const Colorf& value) noexcept {
        button_pressed_color_ = value;
    }

    constexpr const Colorf& disabledTextColor() const noexcept {
        return disabled_text_color_;
    }
    constexpr void setDisabledTextColor(const Colorf& value) noexcept {
        disabled_text_color_ = value;
    }
    constexpr const Colorf& backgroundColor() const noexcept {
        return background_color_;
    }
    constexpr void setBackgroundColor(const Colorf& value) noexcept {
        background_color_ = value;
    }
    constexpr const Colorf& borderColor() const noexcept {
        return border_color_;
    }
    constexpr void setBorderColor(const Colorf& value) noexcept {
        border_color_ = value;
    }
    constexpr const Colorf& imageColor() const noexcept {
        return image_color_;
    }
    constexpr void setImageColor(const Colorf& value) noexcept {
        image_color_ = value;
    }
    constexpr float fontSize() const noexcept {
        return font_size_;
    }
    constexpr void setFontSize(float value) {
        if (!(value >= 1 && value <= 4096)) {
            throw std::invalid_argument("Invalid GUI font size");
        }
        font_size_ = value;
    }
    // A multiplier of the font size, shared by text measurement and drawing.
    constexpr float lineHeight() const noexcept {
        return line_height_;
    }
    constexpr void setLineHeight(float value) {
        if (!(value > 0) || !std::isfinite(value * 4096.0f)) {
            throw std::invalid_argument("Invalid GUI line height");
        }
        line_height_ = value;
    }
    constexpr float borderWidth() const noexcept {
        return border_width_;
    }
    constexpr void setBorderWidth(float value) {
        if (!std::isfinite(value) || value < 0) {
            throw std::invalid_argument("Invalid GUI border width");
        }
        border_width_ = value;
    }

private:
    Colorf disabled_text_color_{0.5f, 0.5f, 0.5f};
    Colorf background_color_ = Colorf::Transparent;
    Colorf border_color_ = Colorf::White;
    Colorf image_color_ = Colorf::White;
    float font_size_ = 16;
    float line_height_ = 1.2f;
    float border_width_ = 0;
    Colorf text_color_ = Colorf::White;
    Colorf button_pressed_color_{0, 0, 0, 0.2f};
};

VOLCANO_GUI_END

#endif
