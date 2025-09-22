//
//
#include <Volcano/World/Dynamic.h>

VOLCANO_WORLD_BEGIN

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

VOLCANO_WORLD_END
