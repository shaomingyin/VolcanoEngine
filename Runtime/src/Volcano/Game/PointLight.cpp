//
//
#include <Volcano/Game/PointLight.hpp>

VOLCANO_GAME_BEGIN

PointLight::PointLight(QObject *parent)
    : Light(parent) {
}

PointLight::~PointLight(void) {
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

VOLCANO_GAME_END
