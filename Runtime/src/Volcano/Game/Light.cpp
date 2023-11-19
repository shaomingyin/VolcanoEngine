//
//
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

Light::Light(QObject *parent)
    : Actor(parent)
    , color_(QColor::fromRgb(255, 255, 255)) {
}

Light::~Light(void) {
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

VOLCANO_GAME_END
