//
//
#include <Volcano/Framework/Physics.h>

VOLCANO_FRAMEWORK_BEGIN

Physics::Physics(World::Scene& scene)
    : scene_(scene)
    , enabled_(true)
    , bt_collision_dispatcher_(&bt_collision_configuration_)
    , bt_world_(&bt_collision_dispatcher_, &bt_broad_phase_, &bt_solver_, &bt_collision_configuration_) {
    // scene_.on_construct<RigidBocy>().
}

VOLCANO_FRAMEWORK_END
