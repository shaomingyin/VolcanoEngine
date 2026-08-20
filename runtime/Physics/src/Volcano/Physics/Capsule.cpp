//
//
#include <Volcano/World/Capsule.h>

VOLCANO_WORLD_BEGIN

Capsule::Capsule(QObject* parent)
    : Shape(parent)
    , radius_(1.0f)
    , height_(1.0f) {
}

float Capsule::radius() const noexcept {
    return radius_;
}


void Capsule::setRadius(float v) noexcept {
    if (!qFuzzyCompare(radius_, v)) {
        radius_ = v;
        emit radiusChanged(v);
    }
}

float Capsule::height() const noexcept {
    return height_;
}


void Capsule::setHeight(float v) noexcept {
    if (!qFuzzyCompare(height_, v)) {
        height_ = v;
        emit heightChanged(v);
    }
}

VOLCANO_WORLD_END
