//
//
#include <Volcano/World/Cone.h>

VOLCANO_WORLD_BEGIN

Cone::Cone(QObject* parent)
    : Shape(parent)
    , radius_(1.0f)
    , height_(1.0f) {
}

float Cone::radius() const noexcept {
    return radius_;
}

void Cone::setRadius(float v) noexcept {
    if (!qFuzzyCompare(radius_, v)) {
        radius_ = v;
        emit radiusChanged(v);
    }
}

float Cone::height() const noexcept {
    return height_;
}

void Cone::setHeight(float v) noexcept {
    if (!qFuzzyCompare(height_, v)) {
        height_ = v;
        emit heightChanged(v);
    }
}

VOLCANO_WORLD_END
