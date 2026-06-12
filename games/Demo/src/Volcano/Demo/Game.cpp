//
//
#include <Volcano/Demo/Game.h>

VOLCANO_DEMO_BEGIN

Game::Game()
    : physics_system_(*this)
    , player_(create()) {
    emplace<World::
}

Game::~Game() {
}

const std::string& Game::name() const noexcept {
    static std::string n("Demo");
    return n;
}

entt::entity Game::mainCamera() const noexcept {
    return player_;
}

void Game::update(Clock::duration elapsed) noexcept {
}

VOLCANO_DEMO_END

std::unique_ptr<Volcano::World::Scene> createVolcanoWorldScene() {
    return std::make_unique<Volcano::Demo::Game>();
}
