//
//
#include <Volcano/Game/Camera.h>

VOLCANO_GAME_BEGIN

Camera::Camera(QObject* parent)
    : Transformable(parent) {
    view_matrix_.setToIdentity();
    projection_matrix_.setToIdentity();
}

VOLCANO_GAME_END
