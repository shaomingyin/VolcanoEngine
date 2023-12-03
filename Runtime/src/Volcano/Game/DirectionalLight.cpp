//
//
#include <Volcano/Game/DirectionalLight.hpp>

VOLCANO_GAME_BEGIN

DirectionalLight::DirectionalLight(QObject* parent)
    : Light(parent)
    , direction_(0.0f, -1.0f, 0.0f) {
}

DirectionalLight::DirectionalLight(Context& context, QObject* parent)
    : Light(context, parent)
    , direction_(0.0f, -1.0f, 0.0f) {
}

const QVector3D& DirectionalLight::direction(void) const {
    return direction_;
}

void DirectionalLight::setDirection(const QVector3D& v) {
    if (!qFuzzyCompare(direction_, v)) {
        direction_ = v;
        emit directionChanged(v);
    }
}

VOLCANO_GAME_END
