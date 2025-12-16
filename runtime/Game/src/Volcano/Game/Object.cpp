//
//
#include <QtQml>

#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

Object::Object(QObject* parent)
    : QObject(parent)
    , context_(nullptr) {
}

Object::Object(Context& context, QObject* parent)
    : QObject(parent)
    , context_(&context) {
}

Context& Object::context() noexcept {
    if (Q_LIKELY(context_ != nullptr)) {
        return *context_;
    }
    auto qml_engine = qmlEngine(this);
    Q_ASSERT(qml_engine != nullptr);
    context_ = Context::fromQmlEngine(*qml_engine);
    assert(context_ != nullptr);
    return *context_;
}

QNetworkAccessManager* Object::networkAccessManager() {
    return context().networkAccessManager();
}

QDataStream& operator<<(QDataStream& s, const Object& v) {
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Object& v) {
    // TODO
    return s;
}

VOLCANO_GAME_END
