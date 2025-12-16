//
//
#include <QVariant>

#include <Volcano/Game/Context.h>

VOLCANO_GAME_BEGIN

static const char PropName[] = "__VOLCANO_CONTEXT__";

void Context::attachToQmlEngine(QQmlEngine& qml_engine) {
    qml_engine.setProperty(PropName, QVariant(reinterpret_cast<qulonglong>(this)));
}

Context* Context::fromQmlEngine(QQmlEngine& qml_engine) {
    auto v = qml_engine.property(PropName);
    if (v.isValid()) {
        auto i = v.value<qlonglong>();
        auto p = reinterpret_cast<Context*>(i);
        if (p != nullptr) {
            return p;
        }
    }
    return nullptr;
}

VOLCANO_GAME_END
