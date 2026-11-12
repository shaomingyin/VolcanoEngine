#include <algorithm>
#include <cmath>
#include <stdexcept>

#include <Volcano/Gui/ProgressBar.h>

VOLCANO_GUI_BEGIN

void ProgressBar::setValue(float value) {
    if (!std::isfinite(value)) {
        throw std::invalid_argument("Invalid GUI progress value");
    }
    value = std::clamp(value, 0.0f, 1.0f);
    if (value_ != value) {
        value_ = value;
        changed();
    }
}

Sizef ProgressBar::measure(Platform& platform, Sizef available) const {
    auto size = Widget::measure(platform, available);
    if (widthSizing() != Sizing::Fixed) {
        size.width = widthSizing() == Sizing::Fill ? available.width : platform.style().fontSize() * 8;
    }
    if (heightSizing() != Sizing::Fixed) {
        size.height = heightSizing() == Sizing::Fill ? available.height : platform.style().fontSize();
    }
    return size;
}

void ProgressBar::paint(World::Canvas& canvas, Platform& platform) const {
    Widget::paint(canvas, platform);
    if (bounds().empty()) {
        return;
    }
    const float width = std::min({1.0f, bounds().width, bounds().height});
    const auto& style = platform.style();
    canvas.setColor(root().acceptsInput(*this) ? style.textColor() : style.disabledTextColor());
    canvas.beginPath();
    canvas.rect({bounds().left + width * 0.5f, bounds().top + width * 0.5f, bounds().width - width,
                   bounds().height - width});
    canvas.stroke(width);
    if (value_ > 0 && bounds().width > width * 2 && bounds().height > width * 2) {
        canvas.beginPath();
        canvas.rect({bounds().left + width, bounds().top + width, (bounds().width - width * 2) * value_,
                       bounds().height - width * 2});
        canvas.fill();
    }
}

VOLCANO_GUI_END
