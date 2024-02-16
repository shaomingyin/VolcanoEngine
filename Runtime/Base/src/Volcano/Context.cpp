//
//
#include <Volcano/Context.hpp>

VOLCANO_BEGIN

const char ContextPropertyName[] = "__VOLCANO_CONTEXT__";

Context::Context(View& view, QObject* parent)
    : QObject(parent)
    , view_(view) {
}

void Context::attachToQmlEngine(QQmlEngine& qml_engine) {
    qml_engine.setProperty(ContextPropertyName, reinterpret_cast<quintptr>(this));
}

Context* Context::fromQmlEngine(const QQmlEngine& qml_engine) {
    auto var = qml_engine.property(ContextPropertyName);
    if (var.canConvert<quintptr>()) {
        return reinterpret_cast<Context*>(var.value<quintptr>());
    }
    return nullptr;
}

VOLCANO_END
