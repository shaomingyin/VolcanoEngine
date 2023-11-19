//
//
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

Actor::Actor(QObject *parent)
    : Object(parent) {
}

Actor::~Actor(void) {
}

const QVector3D &Actor::position() const {
    return position_;
}

void Actor::setPosition(const QVector3D &v) {
    if (position_ != v) {
        position_ = v;
        emit positionChanged(v);
    }
}

const QVector3D &Actor::scale() const {
    return scale_;
}

void Actor::setScale(const QVector3D &v) {
    if (scale_ != v) {
        scale_ = v;
        emit scaleChanged(v);
    }
}

const QQuaternion &Actor::rotation() const {
    return rotation_;
}

void Actor::setRotation(const QQuaternion &v) {
    if (rotation_ != v) {
        rotation_ = v;
        emit rotationChanged(v);
    }
}

VOLCANO_GAME_END
