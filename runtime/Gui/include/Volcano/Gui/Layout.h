#ifndef VOLCANO_GUI_LAYOUT_H
#define VOLCANO_GUI_LAYOUT_H

#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

class Layout final : public Widget {
public:
    using Widget::Widget;
    enum class Mode { Absolute, Horizontal, Vertical };
    enum class Justify { Start, Center, End, SpaceBetween };
    void setLayout(Mode value);
    Mode layoutMode() const noexcept {
        return layout_;
    }
    void setPadding(float value);
    void setPadding(const Insets& value);
    const Insets& padding() const noexcept {
        return padding_;
    }
    void setGap(float value);
    void setGap(float horizontal, float vertical);
    float horizontalGap() const noexcept {
        return horizontal_gap_;
    }
    float verticalGap() const noexcept {
        return vertical_gap_;
    }
    void setJustify(Justify value);
    Justify justify() const noexcept {
        return justify_;
    }
    void setAlignment(Alignment value);
    Alignment alignment() const noexcept {
        return alignment_;
    }
    void setWrap(bool value);
    bool wrap() const noexcept {
        return wrap_;
    }
    Sizef measure(Platform& platform, Sizef available) const override;
    void layout(const Rectf& bounds, Platform& platform) override;

private:
    Sizef calculate(Platform& platform, Sizef available, bool arrange) const;

private:
    Mode layout_ = Mode::Absolute;
    Insets padding_;
    float horizontal_gap_ = 0;
    float vertical_gap_ = 0;
    Justify justify_ = Justify::Start;
    Alignment alignment_ = Alignment::Start;
    bool wrap_ = false;
};

VOLCANO_GUI_END

#endif
