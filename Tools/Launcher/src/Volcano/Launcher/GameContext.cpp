//
//
#include <Volcano/Launcher/GameContext.hpp>

VOLCANO_LAUNCHER_BEGIN

GameContext::GameContext(QObject* parent)
    : Game::Context(parent)
    , view_drawing_(0) {
}

ResourceManager& GameContext::resourceManager() {
    return resource_manager_;
}

bool GameContext::beginView() {
    return false;
}

void GameContext::endView() {

}

Graphics::View& GameContext::view() {
    return views_[view_drawing_];
}

VOLCANO_LAUNCHER_END
