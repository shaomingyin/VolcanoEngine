//
//
#include <Volcano/Demo/Game.h>

VOLCANO_DEMO_BEGIN

Game::Game(const nlohmann::json& metadata)
    : World::Scene(metadata)
    , physics_(*this) {
    auto gravity_it = metadata.find("gravity");
    if (gravity_it != metadata.end()) {
        physics_.setGravity(gravity_it->get<Eigen::Vector3f>());
    }

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
