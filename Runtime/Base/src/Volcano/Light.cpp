//
//
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

Light::Light(QObject *parent)
    : Actor(parent)
    , color_(QColor::fromRgb(255, 255, 255)) {
}

Light::Light(Context& context, QObject *parent)
    : Actor(context, parent)
    , color_(QColor::fromRgb(255, 255, 255)) {
}

const QColor &Light::color(void) const {
    return color_;
}

void Light::setColor(const QColor &v) {
    if (color_ != v) {
        color_ = v;
        emit colorChanged(v);
    }
}

VOLCANO_END
