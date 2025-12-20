//
//
#include <QtQml>

#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

Object::Object(QObject* parent)
    : QObject(parent)
    , context_(nullptr) {
}

Context& Object::context() noexcept {
    Q_ASSERT(context_ != nullptr);
    return *context_;
}

void Object::classBegin() {
}

void Object::componentComplete() {
    context_ = Context::fromQmlObject(this);
}

QNetworkAccessManager* Object::networkAccessManager() {
    return context().networkAccessManager();
}

QString Object::toQml(const QMetaObject& meta_object, const QString& url) {
    if (url.isEmpty()) {
        return QString("%1 { }").arg(meta_object.className());
    }
    return QString("import %1; %2 { }").arg(url).arg(meta_object.className());
}

std::unique_ptr<QQmlComponent> Object::toQmlComponent(QQmlEngine& qml_engine, const QMetaObject& meta_object, const QString& url) {
    auto qml_component = std::make_unique<QQmlComponent>(&qml_engine);
    qml_component->setData(toQml(meta_object, url).toUtf8(), url);
    return qml_component;
}

VOLCANO_GAME_END
