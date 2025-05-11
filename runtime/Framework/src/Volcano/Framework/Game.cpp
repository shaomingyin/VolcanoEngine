//
//
#include <Volcano/Framework/Game.h>

VOLCANO_FRAMEWORK_BEGIN

Game::Game(Context& context)
    : context_(context) {
}

const std::string& Game::name() {
    return "NoNamed";
}

const VersionNumber& Game::version() const {
    return VersionNumber();
}

void Game::loadConfig(const nlohmann::json& j) {
}

void Game::loadScene(const nlohmann::json& j) {
}

void Game::event(const SDL_Event& evt) {

}

void Game::frame(Duration elapsed) {
}

VOLCANO_FRAMEWORK_END
