//
//
#include <QtQml>

#include <Volcano/World/Object.h>

VOLCANO_WORLD_BEGIN

Object::Object(QObject* parent)
    : QObject(parent)
    , context_(nullptr) {
    if (parent != nullptr) {
        auto parent_object = qobject_cast<Object*>(parent);
        if (parent_object != nullptr) {
            context_ = parent_object->context_;
        }
    }
}

Object::Object(Context& context, QObject* parent)
    : QObject(parent)
    , context_(&context) {
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

QNetworkAccessManager* Object::networkAccessManager() {
    if (context_ != nullptr) {
        return context_->networkAccessManager(this);
    }
    return nullptr;
}

VOLCANO_WORLD_END
