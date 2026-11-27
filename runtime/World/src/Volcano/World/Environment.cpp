#include <Volcano/World/Environment.h>

VOLCANO_WORLD_BEGIN

Environment::Environment(QObject* parent)
    : QObject(parent)
    , ambientLight_(this) {
    ambientLight_.setColor(Qt::white);
    ambientLight_.setStrength(0.0f);
}

Light* Environment::ambientLight() noexcept {
    return &ambientLight_;
}

VOLCANO_WORLD_END
