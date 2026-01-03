//
//
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

Scene::Scene()
    : dynamic_enabled_(true)
    , gravity_(0.0f, -9.8f, 0.0f) {
}

void Scene::clear() noexcept {

}

void Scene::update(Clock::duration elapsed) {
    if (dynamic_enabled_) {
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        auto elapsed_ms_double = double(elapsed_ms) / 1000;
    }
}

VOLCANO_GAME_END
