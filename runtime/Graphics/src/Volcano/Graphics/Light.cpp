//
//
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

Light::Light(QObject* parent)
    : Transformable(parent)
    , color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

const QColor& Light::color() const noexcept {
    return color_;
}

void Light::setColor(const QColor& v) noexcept {
    color_ = v;
    emit colorChanged(v);
}

float Light::strength() const noexcept {
    return strength_;
}

void Light::setStrength(float v) noexcept {
    if (!qFuzzyCompare(strength_, v)) {
        strength_ = v;
        emit strengthChanged(v);
    }
}

VOLCANO_WORLD_END
