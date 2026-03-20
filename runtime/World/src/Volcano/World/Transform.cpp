//
//
#include <Volcano/World/Transform.h>

VOLCANO_WORLD_BEGIN

const Transform Transform::Identity;

Transform::Transform(QObject* parent)
    : QObject(parent)
    , translation_(0.0f, 0.0f, 0.0f)
    , scale_(1.0f, 1.0f, 1.0f)
    , rotation_(1.0f, 0.0f, 0.0f, 0.0f) {
}

VOLCANO_WORLD_END
