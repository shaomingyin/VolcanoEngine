//
//
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

Object::Object(QObject *parent)
    : QObject(parent)
    , context_(nullptr) {
}

Object::Object(Context& context, QObject *parent)
    : QObject(parent)
    , context_(&context) {
}

Context* Object::context() {
    if (Q_LIKELY(context_ != nullptr)) {
        return context_;
    }
    auto qml_engine = qmlEngine(this);
    if (qml_engine != nullptr) {
        context_ = Context::fromQmlEngine(*qml_engine);
        if (context_ == nullptr) {
            qWarning("Failed to get context, object [%p] is not a QML object.", this);
        }
    }
    return context_;
}

VOLCANO_GAME_END
