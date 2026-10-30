#include <cmath>
#include <stdexcept>
#include <utility>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Gui/Document.h>

VOLCANO_GUI_BEGIN

Document::Document(Platform& platform, std::unique_ptr<Layout> root)
    : platform_(platform), root_(std::move(root)) {
    if (!root_) {
        throw std::invalid_argument("GUI document requires a root container");
    }
    VOLCANO_ASSERT(root_->parent() == nullptr);
    root_->observe(*this);
}

Document::~Document() {
    root_->unobserve(*this);
}

void Document::remove(Widget& subtree) {
    auto* captured = captured_ && subtree.contains(*captured_) ? captured_ : nullptr;
    auto* hovered = hovered_ && subtree.contains(*hovered_) ? hovered_ : nullptr;
    auto* focused = focus_ && subtree.contains(*focus_) ? focus_ : nullptr;
    destroy(subtree);

    dispatching_ = true;
    auto restore = scopeGuard([this] {
        dispatching_ = false;
    });
    if (captured) {
        auto input = captured_event_;
        input.action = PointerEvent::Action::Cancel;
        input.inside = false;
        captured->event(input, platform_);
    }
    if (hovered) {
        auto input = hovered_event_;
        input.action = PointerEvent::Action::Leave;
        input.inside = false;
        hovered->event(input, platform_);
    }
    if (focused) {
        focused->event(FocusEvent{false}, platform_);
    }
}

void Document::destroy(Widget& subtree) noexcept {
    if (captured_ && subtree.contains(*captured_)) {
        captured_ = nullptr;
    }
    if (hovered_ && subtree.contains(*hovered_)) {
        hovered_ = nullptr;
    }
    if (focus_ && subtree.contains(*focus_)) {
        focus_ = nullptr;
    }
}

void Document::layout(Sizef size) {
    VOLCANO_ASSERT(!dispatching_);
    dispatching_ = true;
    auto restore = scopeGuard([this] {
        dispatching_ = false;
    });
    if (!(size.width >= 0 && size.height >= 0) || !std::isfinite(size.width) || !std::isfinite(size.height)) {
        throw std::invalid_argument("Invalid GUI layout size");
    }
    root_->layout({0, 0, size.width, size.height}, platform_);
}

void Document::draw() const {
    VOLCANO_ASSERT(!dispatching_);
    dispatching_ = true;
    auto restore = scopeGuard([this] {
        dispatching_ = false;
    });
    root_->draw(platform_);
}

void Document::setFocus(Widget* widget) {
    if (focus_ == widget) {
        return;
    }
    auto* previous = focus_;
    focus_ = widget;
    if (previous) {
        previous->event(FocusEvent{false}, platform_);
    }
    if (focus_) {
        focus_->event(FocusEvent{true}, platform_);
    }
}

bool Document::setHover(Widget* widget, const PointerEvent& input) {
    hovered_event_ = input;
    if (hovered_ == widget) {
        return false;
    }
    auto* previous = hovered_;
    hovered_ = widget;
    auto routed = input;
    bool handled = false;
    if (previous) {
        routed.action = PointerEvent::Action::Leave;
        routed.inside = false;
        handled = previous->event(routed, platform_);
    }
    if (hovered_) {
        routed.action = PointerEvent::Action::Enter;
        routed.inside = true;
        handled = hovered_->event(routed, platform_) || handled;
    }
    return handled;
}

bool Document::cancelCapture() {
    auto* captured = captured_;
    captured_ = nullptr;
    if (!captured) {
        return false;
    }
    auto cancelled = captured_event_;
    cancelled.action = PointerEvent::Action::Cancel;
    cancelled.inside = false;
    captured->event(cancelled, platform_);
    return true;
}

Widget* Document::focusedWidget() {
    if (focus_ && !root_->acceptsInput(*focus_)) {
        cancelCapture();
        setFocus(nullptr);
    }
    return active_ ? focus_ : nullptr;
}

bool Document::event(const PointerEvent& input) {
    VOLCANO_ASSERT(!dispatching_);
    dispatching_ = true;
    auto restore = scopeGuard([this] {
        dispatching_ = false;
    });
    if (!std::isfinite(input.x) || !std::isfinite(input.y)) {
        throw std::invalid_argument("Invalid GUI pointer position");
    }
    if (!active_ ||
        (captured_ && (input.id != captured_event_.id || input.device != captured_event_.device))) {
        return false;
    }
    if (input.action == PointerEvent::Action::Cancel) {
        return cancelCapture();
    }
    auto* hit = input.action == PointerEvent::Action::Leave ? nullptr : root_->hit(input.x, input.y);
    bool hoverHandled = false;
    if (input.device != PointerEvent::Device::Touch) {
        hoverHandled = setHover(hit, input);
    }
    if (captured_) {
        if (input.action == PointerEvent::Action::Down ||
            (input.action == PointerEvent::Action::Up && input.button != captured_event_.button)) {
            return false;
        }
        if (!root_->acceptsInput(*captured_)) {
            return cancelCapture();
        }
        auto* target = captured_;
        auto routed = input;
        routed.inside = hit == target && input.action != PointerEvent::Action::Leave;
        captured_event_.x = input.x;
        captured_event_.y = input.y;
        captured_event_.modifiers = input.modifiers;
        if (input.action == PointerEvent::Action::Up) {
            captured_ = nullptr;
        }
        return target->event(routed, platform_);
    }
    if (input.action == PointerEvent::Action::Down && input.button == PointerEvent::Button::Primary) {
        setFocus(hit && hit->focusable() ? hit : nullptr);
    }
    if (input.action == PointerEvent::Action::Enter || input.action == PointerEvent::Action::Leave) {
        return hoverHandled;
    }
    if (!hit || input.action == PointerEvent::Action::Up) {
        return false;
    }
    auto routed = input;
    routed.inside = input.action != PointerEvent::Action::Leave;
    const bool handled = hit->event(routed, platform_);
    if (handled && input.action == PointerEvent::Action::Down) {
        captured_ = hit;
        captured_event_ = routed;
    }
    return handled;
}

bool Document::event(const ScrollEvent& input) {
    VOLCANO_ASSERT(!dispatching_);
    dispatching_ = true;
    auto restore = scopeGuard([this] {
        dispatching_ = false;
    });
    if (!std::isfinite(input.x) || !std::isfinite(input.y) || !std::isfinite(input.deltaX) ||
        !std::isfinite(input.deltaY)) {
        throw std::invalid_argument("Invalid GUI scroll event");
    }
    auto* hit = active_ ? root_->hit(input.x, input.y) : nullptr;
    return hit && hit->event(input, platform_);
}

bool Document::event(const KeyEvent& input) {
    VOLCANO_ASSERT(!dispatching_);
    dispatching_ = true;
    auto restore = scopeGuard([this] {
        dispatching_ = false;
    });
    auto* target = focusedWidget();
    return target && target->event(input, platform_);
}

bool Document::event(const TextInputEvent& input) {
    VOLCANO_ASSERT(!dispatching_);
    dispatching_ = true;
    auto restore = scopeGuard([this] {
        dispatching_ = false;
    });
    auto* target = focusedWidget();
    return target && target->event(input, platform_);
}

bool Document::event(const CompositionEvent& input) {
    VOLCANO_ASSERT(!dispatching_);
    dispatching_ = true;
    auto restore = scopeGuard([this] {
        dispatching_ = false;
    });
    const auto size = input.text.size();
    if (input.selectionStart > size || input.selectionLength > size - input.selectionStart) {
        throw std::invalid_argument("Invalid GUI composition selection");
    }
    const auto boundary = [&](std::size_t offset) {
        return offset == size || (static_cast<unsigned char>(input.text[offset]) & 0xc0) != 0x80;
    };
    if (!boundary(input.selectionStart) || !boundary(input.selectionStart + input.selectionLength)) {
        throw std::invalid_argument("GUI composition selection splits a UTF-8 code point");
    }
    auto* target = focusedWidget();
    return target && target->event(input, platform_);
}

bool Document::event(const FocusEvent& input) {
    VOLCANO_ASSERT(!dispatching_);
    dispatching_ = true;
    auto restore = scopeGuard([this] {
        dispatching_ = false;
    });
    active_ = input.focused;
    if (!active_) {
        const bool hadFocus = focus_ != nullptr;
        const bool captured = cancelCapture();
        setHover(nullptr, hovered_event_);
        setFocus(nullptr);
        return hadFocus || captured;
    }
    return false;
}

VOLCANO_GUI_END
