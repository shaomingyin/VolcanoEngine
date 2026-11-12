#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

#include <Volcano/Gui/Slider.h>

VOLCANO_GUI_BEGIN

void Slider::setRange(float minimum, float maximum) {
    if (!std::isfinite(minimum) || !std::isfinite(maximum) || minimum >= maximum) {
        throw std::invalid_argument("Invalid GUI slider range");
    }
    if (minimum_ == minimum && maximum_ == maximum) {
        return;
    }
    minimum_ = minimum;
    maximum_ = maximum;
    changed();
    setValue(value_);
}

void Slider::setValue(float value) {
    if (!std::isfinite(value)) {
        throw std::invalid_argument("Invalid GUI slider value");
    }
    value = std::clamp(value, minimum_, maximum_);
    if (value_ == value) {
        return;
    }
    value_ = value;
    changed();
    auto callback = on_value_changed_;
    if (callback) {
        callback(value);
    }
}

void Slider::setStep(float value) {
    if (!std::isfinite(value) || value <= 0) {
        throw std::invalid_argument("Invalid GUI slider step");
    }
    step_ = value;
}

void Slider::setOnValueChanged(std::function<void(float)> callback) {
    on_value_changed_ = std::move(callback);
}

void Slider::setPosition(float x) {
    const float side = std::min(bounds().width, bounds().height);
    const float travel = bounds().width - side;
    if (travel == 0) {
        return;
    }
    const auto fraction = std::clamp((double(x) - bounds().left - side * 0.5) / travel, 0.0, 1.0);
    setValue(std::lerp(minimum_, maximum_, static_cast<float>(fraction)));
}

bool Slider::event(const PointerEvent& input, Platform&) {
    if (input.action == PointerEvent::Action::Cancel) {
        return std::exchange(dragging_, false);
    }
    if (!enabled() || !visible()) {
        dragging_ = false;
        return false;
    }
    if (input.action == PointerEvent::Action::Down) {
        if (input.button != PointerEvent::Button::Primary || bounds().empty() ||
            bounds().width <= bounds().height) {
            return false;
        }
        setPosition(input.x);
        dragging_ = true;
        return true;
    }
    if (input.action == PointerEvent::Action::Move && dragging_) {
        setPosition(input.x);
        return true;
    }
    if (input.action == PointerEvent::Action::Up && dragging_ &&
        input.button == PointerEvent::Button::Primary) {
        dragging_ = false;
        setPosition(input.x);
        return true;
    }
    return false;
}

bool Slider::event(const KeyEvent& input, Platform&) {
    if (!enabled() || !visible() || input.modifiers.control || input.modifiers.alt || input.modifiers.super) {
        return false;
    }
    double next = value_;
    switch (input.key) {
    case KeyEvent::Key::Left:
    case KeyEvent::Key::Down:
        next -= step_;
        break;
    case KeyEvent::Key::Right:
    case KeyEvent::Key::Up:
        next += step_;
        break;
    case KeyEvent::Key::Home:
        next = minimum_;
        break;
    case KeyEvent::Key::End:
        next = maximum_;
        break;
    default:
        return false;
    }
    if (input.action != KeyEvent::Action::Up) {
        setValue(static_cast<float>(std::clamp(next, double(minimum_), double(maximum_))));
    }
    return true;
}

bool Slider::event(const FocusEvent& input, Platform&) {
    if (!input.focused) {
        dragging_ = false;
    }
    return false;
}

Sizef Slider::measure(Platform& platform, Sizef available) const {
    auto size = Widget::measure(platform, available);
    if (widthSizing() != Sizing::Fixed) {
        size.width = widthSizing() == Sizing::Fill ? available.width : platform.style().fontSize() * 8;
    }
    if (heightSizing() != Sizing::Fixed) {
        size.height = heightSizing() == Sizing::Fill ? available.height : platform.style().fontSize();
    }
    return size;
}

void Slider::paint(World::Canvas& canvas, Platform& platform) const {
    Widget::paint(canvas, platform);
    if (bounds().empty()) {
        return;
    }
    const auto& style = platform.style();
    const float side = std::min(bounds().width, bounds().height);
    const float trackHeight = std::min(4.0f, bounds().height);
    canvas.setColor(style.disabledTextColor());
    canvas.beginPath();
    canvas.rect(
        {bounds().left, bounds().top + (bounds().height - trackHeight) * 0.5f, bounds().width, trackHeight});
    canvas.fill();
    const double fraction = (double(value_) - minimum_) / (double(maximum_) - minimum_);
    const float x = bounds().left + static_cast<float>(fraction * (bounds().width - side));
    canvas.setColor(root().acceptsInput(*this) ? style.textColor() : style.disabledTextColor());
    canvas.beginPath();
    canvas.rect({x, bounds().top + (bounds().height - side) * 0.5f, side, side});
    canvas.fill();
}

VOLCANO_GUI_END
