//
//
#include <Volcano/World/Sphere.h>

VOLCANO_WORLD_BEGIN

Sphere::Sphere(QObject* parent)
    : Shape(parent)
    , radius_(1.0f) {
}

float Sphere::radius() const noexcept {
    return radius_;
}

void Sphere::setRadius(float v) noexcept {
    if (!qFuzzyCompare(radius_, v)) {
        radius_ = v;
        emit radiusChanged(v);
    }
}

VOLCANO_WORLD_END
