#include <algorithm>
#include <utility>

#include <Volcano/Gui/CheckBox.h>

VOLCANO_GUI_BEGIN

void CheckBox::setChecked(bool value) {
    if (checked_ == value) {
        return;
    }
    checked_ = value;
    changed();
    auto callback = on_toggled_;
    if (callback) {
        callback(value);
    }
}

void CheckBox::setOnToggled(std::function<void(bool)> callback) {
    on_toggled_ = std::move(callback);
}

bool CheckBox::event(const PointerEvent& input, Platform&) {
    if (input.action == PointerEvent::Action::Cancel) {
        return std::exchange(pointer_down_, false);
    }
    if (!enabled() || !visible()) {
        pointer_down_ = false;
        return false;
    }
    if (input.action == PointerEvent::Action::Down) {
        if (input.button != PointerEvent::Button::Primary) {
            return false;
        }
        pointer_down_ = true;
        return true;
    }
    if (input.action == PointerEvent::Action::Up) {
        if (!pointer_down_ || input.button != PointerEvent::Button::Primary) {
            return false;
        }
        pointer_down_ = false;
        if (input.inside) {
            setChecked(!checked_);
        }
        return true;
    }
    return pointer_down_ && input.action == PointerEvent::Action::Move;
}

bool CheckBox::event(const KeyEvent& input, Platform&) {
    if (!enabled() || !visible() || input.modifiers.control || input.modifiers.alt || input.modifiers.super ||
        input.key != KeyEvent::Key::Space) {
        return false;
    }
    if (input.action == KeyEvent::Action::Down) {
        setChecked(!checked_);
    }
    return true;
}

bool CheckBox::event(const FocusEvent& input, Platform&) {
    if (!input.focused) {
        pointer_down_ = false;
    }
    return false;
}

Sizef CheckBox::measure(Platform& platform, Sizef available) const {
    auto size = Widget::measure(platform, available);
    if (widthSizing() != Sizing::Fixed) {
        size.width = widthSizing() == Sizing::Fill ? available.width : platform.style().fontSize();
    }
    if (heightSizing() != Sizing::Fixed) {
        size.height = heightSizing() == Sizing::Fill ? available.height : platform.style().fontSize();
    }
    return size;
}

void CheckBox::paint(Platform& platform) const {
    Widget::paint(platform);
    const float side = std::min(bounds().width, bounds().height);
    if (side == 0) {
        return;
    }
    const float width = std::min(side, std::max(1.0f, side / 8));
    const Rectf box{bounds().left + (bounds().width - side) * 0.5f,
                    bounds().top + (bounds().height - side) * 0.5f, side, side};
    const auto& style = platform.style();
    platform.setColor(root().acceptsInput(*this) ? style.textColor() : style.disabledTextColor());
    platform.beginPath();
    platform.rect({box.left + width * 0.5f, box.top + width * 0.5f, side - width, side - width});
    platform.stroke(width);
    if (checked_) {
        platform.beginPath();
        platform.moveTo(box.left + side * 0.2f, box.top + side * 0.5f);
        platform.lineTo(box.left + side * 0.4f, box.top + side * 0.7f);
        platform.lineTo(box.left + side * 0.8f, box.top + side * 0.3f);
        platform.stroke(width);
    }
}

VOLCANO_GUI_END
