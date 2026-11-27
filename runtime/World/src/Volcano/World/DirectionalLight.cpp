//
//
#include <Volcano/World/DirectionalLight.h>

VOLCANO_WORLD_BEGIN

DirectionalLight::DirectionalLight(QObject* parent)
    : Light(parent) {
}

const QVector3D& DirectionalLight::direction() const noexcept {
    return direction_;
}

void DirectionalLight::setDirection(const QVector3D& v) noexcept {
    if (!qFuzzyCompare(direction_, v)) {
        direction_ = v;
        emit directionChanged(v);
    }
}

VOLCANO_WORLD_END
