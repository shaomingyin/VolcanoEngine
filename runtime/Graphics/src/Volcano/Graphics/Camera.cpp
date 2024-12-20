//
//
#include <Volcano/Graphics/Camera.h>

VOLCANO_GRAPHICS_BEGIN

Camera::Camera(QObject* parent)
    : World::Transformable(parent) {
    view_.setToIdentity();
    projection_.setToIdentity();
}

VOLCANO_GRAPHICS_END
