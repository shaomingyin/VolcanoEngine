#ifndef VOLCANO_GUI_BUTTON_H
#define VOLCANO_GUI_BUTTON_H

#include <functional>

#include <Volcano/Gui/Text.h>

VOLCANO_GUI_BEGIN

class Button final : public Text {
public:
    using Text::Text;
    bool focusable() const noexcept override {
        return true;
    }
    void setPressed(bool value);
    bool pressed() const noexcept {
        return pressed_;
    }
    void setOnClick(std::function<void()> callback);
    void activate();
    using Text::event;
    bool event(const PointerEvent& event, Platform& platform) override;
    bool event(const KeyEvent& event, Platform& platform) override;
    bool event(const FocusEvent& event, Platform& platform) override;

protected:
    void paint(World::Canvas& canvas, Platform& platform) const override;

private:
    std::function<void()> on_click_;
    bool pressed_ = false;
    bool pointer_down_ = false;
};

VOLCANO_GUI_END

#endif
