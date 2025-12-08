//
//
#include <Volcano/Game/Dynamic.h>

VOLCANO_GAME_BEGIN

Dynamic::Dynamic(QObject* parent)
    : QObject(parent)
    , enabled_(true)
    , gravity_(0.0f, -9.8f, 0.0f)
    , collision_dispatcher_(btCollisionDispatcher(&collision_configuration_))
    , world_(
          &collision_dispatcher_,
          &overlapping_pair_cache_,
          &sequential_impulse_constraint_solver_,
          &collision_configuration_) {
    world_.setGravity(btVector3(gravity_.x(), gravity_.y(), gravity_.x()));
}

QDataStream& operator<<(QDataStream& s, const Dynamic& v) {
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Dynamic& v) {
    // TODO
    return s;
}

VOLCANO_GAME_END
