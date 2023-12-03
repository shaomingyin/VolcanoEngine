//
//
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

const char PropertyName[] = "__VOLCANO_GAME_CONTEXT__";

Context::Context(View& view, QObject* parent)
    : QObject(parent)
    , view_(view) {
}

void Context::attachToQmlEngine(QQmlEngine& qml_engine) {
    qml_engine.setProperty(PropertyName, reinterpret_cast<quintptr>(this));
}

Context* Context::fromQmlEngine(const QQmlEngine& qml_engine) {
    auto var = qml_engine.property(PropertyName);
    if (var.canConvert<quintptr>()) {
        return reinterpret_cast<Context*>(var.value<quintptr>());
    }
    return nullptr;
}

VOLCANO_GAME_END
