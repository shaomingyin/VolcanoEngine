#include <Volcano/World/ColorEnvironment.h>

VOLCANO_WORLD_BEGIN

ColorEnvironment::ColorEnvironment(QObject* parent)
    : Environment(parent) {
}

const QColor& ColorEnvironment::color() const noexcept {
    return color_;
}

void ColorEnvironment::setColor(const QColor& color) {
    if (color_ != color) {
        color_ = color;
        emit colorChanged(color_);
    }
}

VOLCANO_WORLD_END
