//
//
#include <Volcano/Game.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/Demo/Game.h>

VOLCANO_DEMO_BEGIN

Game::Game() {
    emplace<World::PerspectiveCamera>(create(), 60.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
}

Game::~Game() {
}

void Game::onUpdate(Clock::duration elapsed) noexcept {
}

entt::entity Game::player() const noexcept {
    return entt::null;
}

VOLCANO_DEMO_END

std::unique_ptr<Volcano::World::Scene> volcanoCreateGame() {
    return std::make_unique<Volcano::Demo::Game>();
}
