#ifndef VOLCANO_GUI_CHECKBOX_H
#define VOLCANO_GUI_CHECKBOX_H

#include <functional>

#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

class CheckBox final : public Widget {
public:
    using Widget::Widget;
    bool checked() const noexcept {
        return checked_;
    }
    void setChecked(bool value);
    void setOnToggled(std::function<void(bool)> callback);
    bool focusable() const noexcept override {
        return true;
    }
    using Widget::event;
    bool event(const PointerEvent& event, Platform& platform) override;
    bool event(const KeyEvent& event, Platform& platform) override;
    bool event(const FocusEvent& event, Platform& platform) override;
    Sizef measure(Platform& platform, Sizef available) const override;

protected:
    void paint(World::Canvas& canvas, Platform& platform) const override;

private:
    std::function<void(bool)> on_toggled_;
    bool checked_ = false;
    bool pointer_down_ = false;
};

VOLCANO_GUI_END

#endif
