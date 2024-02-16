//
//
#include <Volcano/PointLight.hpp>

VOLCANO_BEGIN

PointLight::PointLight(QObject *parent)
    : Light(parent) {
}

PointLight::PointLight(Context& context, QObject *parent)
    : Light(context, parent) {
}

const QVector3D &PointLight::position(void) const {
    return position_;
}

void PointLight::setPosition(const QVector3D &v) {
    if (!qFuzzyCompare(position_, v)) {
        position_ = v;
        emit positionChanged(v);
    }
}

VOLCANO_END
