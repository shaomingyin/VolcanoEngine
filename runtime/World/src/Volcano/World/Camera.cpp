//
//
#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

Camera::Camera(QObject* parent)
    : Transformable(parent) {
    view_matrix_.setToIdentity();
    projection_matrix_.setToIdentity();
}

QDataStream& operator<<(QDataStream& s, const Camera& v) {
    s << static_cast<const Transformable&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Camera& v) {
    s >> static_cast<Transformable&>(v);
    // TODO
    return s;
}

VOLCANO_WORLD_END
