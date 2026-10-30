#ifndef VOLCANO_GUI_PROGRESSBAR_H
#define VOLCANO_GUI_PROGRESSBAR_H

#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

class ProgressBar final : public Widget {
public:
    using Widget::Widget;
    // A finite fraction, clamped to [0, 1].
    float value() const noexcept {
        return value_;
    }
    void setValue(float value);
    Sizef measure(Platform& platform, Sizef available) const override;

protected:
    void paint(Platform& platform) const override;

private:
    float value_ = 0;
};

VOLCANO_GUI_END

#endif
