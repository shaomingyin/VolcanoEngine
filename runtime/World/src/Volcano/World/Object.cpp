//
//
#include <QtQml>

#include <Volcano/World/Object.h>

VOLCANO_WORLD_BEGIN

Object::Object(QObject* parent)
    : QObject(parent)
    , context_(nullptr) {
}

Context* Object::context() {
    if (Q_LIKELY(context_ != nullptr)) {
        return context_;
    }

    QQmlContext* qml_context = qmlContext(this);
    if (qml_context != nullptr) {
        QObject* context_object = qml_context->contextObject();
        if (context_object != nullptr) {
            context_ = qobject_cast<Context*>(context_object);
        }
    }

    return context_;
}

VOLCANO_WORLD_END
