//
//
#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

Camera::Camera(QObject* parent)
    : World::Transformable(parent) {
    view_matrix_.setToIdentity();
    projection_matrix_.setToIdentity();
}

VOLCANO_WORLD_END
