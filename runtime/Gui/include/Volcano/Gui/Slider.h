#ifndef VOLCANO_GUI_SLIDER_H
#define VOLCANO_GUI_SLIDER_H

#include <functional>

#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

class Slider final : public Widget {
public:
    using Widget::Widget;
    float minimum() const noexcept {
        return minimum_;
    }
    float maximum() const noexcept {
        return maximum_;
    }
    float value() const noexcept {
        return value_;
    }
    float step() const noexcept {
        return step_;
    }
    void setRange(float minimum, float maximum);
    void setValue(float value);
    void setStep(float value);
    void setOnValueChanged(std::function<void(float)> callback);
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
    void setPosition(float x);

private:
    std::function<void(float)> on_value_changed_;
    float minimum_ = 0;
    float maximum_ = 1;
    float value_ = 0;
    float step_ = 0.01f;
    bool dragging_ = false;
};

VOLCANO_GUI_END

#endif
