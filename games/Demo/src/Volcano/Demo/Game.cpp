//
//
#include <rttr/registration>

#include <Volcano/World/Camera.h>
#include <Volcano/Demo/Game.h>

VOLCANO_DEMO_BEGIN

Game::Game()
    : main_camera_(create()) {
    emplace<World::PerspectiveCamera>(main_camera_, 90.0f, 0.5f, 999.0f, 16.0f / 9.0f);
    physics().enable();
}

Game::~Game() {
}

entt::entity Game::mainCamera() const noexcept {
    return main_camera_;
}

void Game::update(World::Clock::duration elapsed) noexcept {
    World::Scene::update(elapsed);
}

VOLCANO_DEMO_END

RTTR_REGISTRATION {
    rttr::registration::class_<Volcano::Demo::Game>("VolcanoGame")
        .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}
