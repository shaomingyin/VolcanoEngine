//
//
#include <Volcano/Physics/World.hpp>

VOLCANO_PHYSICS_BEGIN

World::World(QObject* parent)
    : QObject(parent)
    , gravity_(0.0f, -9.8f, 0.0f) {
}

const QVector3D &World::gravity(void) const {
    return gravity_;
}

void World::setGravity(const QVector3D &v) {
    if (!qFuzzyCompare(gravity_, v)) {
        gravity_ = v;
        emit gravityChanged(v);
    }
}

VOLCANO_PHYSICS_END
