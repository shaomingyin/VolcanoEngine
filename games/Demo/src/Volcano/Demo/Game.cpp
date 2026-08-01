//
//
#include <Volcano/Demo/Game.h>

VOLCANO_DEMO_BEGIN

Game::Game()
    : physics_(*this) {
}

Game::~Game() {
}

void Game::update(Clock::duration elapsed) noexcept {
}

entt::entity Game::player() const noexcept {
    return entt::null;
}

VOLCANO_DEMO_END

std::unique_ptr<Volcano::World::Scene> volcanoCreateGame() {
    return std::make_unique<Volcano::Demo::Game>();
}
