//
//
#include <Volcano/Game/Scene.h>

VOLCANO_GAME_BEGIN

Scene::Scene(Context& context)
    : Object(context)
    , gravity_(0.0f, -9.8f, 0.0f) {
}

Scene::~Scene() {
}

void Scene::enablePhysics() {
}

void Scene::disablePhysics() {
}

void Scene::setGravity(Eigen::Vector3f v) {
    gravity_ = v;
    if (bt_dynamics_world_) {
        bt_dynamics_world_->setGravity(btVector3(v.x(), v.y(), v.z()));
    }
}

VOLCANO_GAME_END
