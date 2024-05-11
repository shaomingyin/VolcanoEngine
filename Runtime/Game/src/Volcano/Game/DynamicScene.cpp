//
//
#include <Volcano/Game/DynamicScene.hpp>

VOLCANO_GAME_BEGIN

DynamicScene::DynamicScene(QObject* parent)
    : Scene(parent) {
}

DynamicScene::DynamicScene(Context& context, QObject* parent)
    : Scene(context, parent) {
}

void DynamicScene::updateView() const {
    Scene::updateView();
}

void DynamicScene::updateState(Duration elapsed) {
    Scene::updateState(elapsed);
}

VOLCANO_GAME_END
