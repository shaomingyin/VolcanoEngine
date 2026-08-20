//
//
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

Transformable::Transformable(QObject* parent)
    : QObject(parent) {
}

Transform* Transformable::offset() noexcept {
    return &offset_;
}

VOLCANO_WORLD_END
