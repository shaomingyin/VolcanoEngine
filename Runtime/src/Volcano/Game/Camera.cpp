//
//
#include <Volcano/Game/Camera.hpp>

VOLCANO_GAME_BEGIN

Camera::Camera(QObject *parent)
    : Object(parent) {
}

const QVector3D &Camera::position() const {
    return position_;
}

void Camera::setPosition(const QVector3D &v) {
    if (position_ != v) {
        position_ = v;
        emit positionChanged(v);
    }
}

const QVector3D &Camera::direction() const {
    return direction_;
}

void Camera::setDirection(const QVector3D &v) {
    auto normalized = v.normalized();
    if (direction_ != normalized) {
        direction_ = normalized;
        emit directionChanged(normalized);
    }
}

const QVector3D &Camera::up() const {
    return up_;
}

void Camera::setUp(const QVector3D &v) {
    auto normalized = v.normalized();
    if (up_ != normalized) {
        up_ = normalized;
        emit upChanged(normalized);
    }
}

void Camera::lookTo(const QVector3D &position, const QVector3D &direction, const QVector3D &up) {
    setPosition(position);
    setDirection(direction);
    setUp(up);
}

void Camera::toViewMatrix(QMatrix4x4 &out) {
    out.setToIdentity();
    out.lookAt(position_, position_ + direction_, up_);
}

VOLCANO_GAME_END
