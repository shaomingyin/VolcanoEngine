//
//
#include <Volcano/World/Box.h>

VOLCANO_WORLD_BEGIN

Box::Box(QObject* parent)
    : Shape(parent)
    , size_(1.0f, 1.0f, 1.0f) {
}

QVector3D Box::size() const noexcept {
    return size_;
}

float Box::length() const noexcept {
    return size_.x();
}

void Box::setLength(float v) noexcept {
    if (!qFuzzyCompare(size_.x(), v)) {
        size_.setX(v);
        emit lengthChanged(v);
    }
}

float Box::width() const noexcept {
    return size_.y();
}

void Box::setWidth(float v) noexcept {
    if (!qFuzzyCompare(size_.z(), v)) {
        size_.setZ(v);
        emit widthChanged(v);
    }
}

float Box::height() const noexcept {
    return size_.z();
}

void Box::setHeight(float v) noexcept {
    if (!qFuzzyCompare(size_.y(), v)) {
        size_.setY(v);
        emit heightChanged(v);
    }
}

VOLCANO_WORLD_END
