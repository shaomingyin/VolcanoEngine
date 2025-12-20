//
//
#include <Volcano/Editor/Context.h>

VOLCANO_EDITOR_BEGIN

Context::Context(QObject* parent)
    : QObject(parent)
    , game_world_(nullptr) {
    auto game_world_component = Game::Object::toQmlComponent(qml_engine_, Game::World::staticMetaObject);
    auto game_world_object = game_world_component->create();
    game_world_ = qobject_cast<Game::World*>(game_world_object);
    QQmlEngine::setObjectOwnership(game_world_, QQmlEngine::JavaScriptOwnership);
}

VOLCANO_EDITOR_END
