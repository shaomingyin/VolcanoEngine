#include <utility>

#include <Volcano/Gui/Button.h>

VOLCANO_GUI_BEGIN

void Button::setPressed(bool value) {
    if (pressed_ != value) {
        pressed_ = value;
        changed();
    }
}

void Button::setOnClick(std::function<void()> callback) {
    on_click_ = std::move(callback);
}

void Button::activate() {
    if (enabled() && on_click_) {
        on_click_();
    }
}

bool Button::event(const PointerEvent& event, Platform&) {
    if (event.action == PointerEvent::Action::Cancel) {
        const bool wasPressed = pressed_;
        pointer_down_ = false;
        setPressed(false);
        return wasPressed;
    }
    if (event.action == PointerEvent::Action::Down) {
        if (event.button != PointerEvent::Button::Primary || !enabled() || !visible()) {
            return false;
        }
        pointer_down_ = true;
        setPressed(true);
        return true;
    }
    if (event.action == PointerEvent::Action::Move || event.action == PointerEvent::Action::Enter ||
        event.action == PointerEvent::Action::Leave) {
        if (!pointer_down_) {
            return false;
        }
        setPressed(event.inside && enabled() && visible());
        return true;
    }
    if (event.action == PointerEvent::Action::Up) {
        if (!pointer_down_) {
            return false;
        }
        pointer_down_ = false;
        setPressed(false);
        if (event.inside && enabled() && visible()) {
            activate();
        }
        return true;
    }
    return false;
}

bool Button::event(const KeyEvent& event, Platform&) {
    if (!enabled() || !visible() || event.modifiers.control || event.modifiers.alt || event.modifiers.super ||
        (event.key != KeyEvent::Key::Enter && event.key != KeyEvent::Key::Space)) {
        return false;
    }
    if (event.action == KeyEvent::Action::Down) {
        activate();
    }
    return true;
}

bool Button::event(const FocusEvent& event, Platform&) {
    if (!event.focused) {
        pointer_down_ = false;
        setPressed(false);
    }
    return false;
}

void Button::paint(World::Canvas& canvas, Platform& platform) const {
    Text::paint(canvas, platform);
    if (pressed_) {
        canvas.setColor(platform.style().buttonPressedColor());
        canvas.beginPath();
        canvas.rect(bounds());
        canvas.fill();
    }
}

VOLCANO_GUI_END
