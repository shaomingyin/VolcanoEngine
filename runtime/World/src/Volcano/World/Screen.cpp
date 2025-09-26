//
//
#include <QUrl>
#include <QString>

#include <Volcano/World/Screen.h>

VOLCANO_WORLD_BEGIN

Screen::Screen(QObject* parent)
    : Transformable(parent)
    , root_item_(nullptr) {
}

void Screen::setSource(const QVariant& v) {
    if (source_ == v) {
        return;
    }

    if (root_item_ != nullptr) {
        delete root_item_;
    }

    disconnect(component_.get(), &QQmlComponent::statusChanged, this, &Screen::statusChanged);

    if (v.canConvert<QUrl>()) {
        component_.reset(new QQmlComponent(qmlEngine(this), v.value<QUrl>(), QQmlComponent::Asynchronous));
    } else if (v.canConvert<QString>()) {
        component_.reset(new QQmlComponent(qmlEngine(this), v.value<QString>(), QQmlComponent::Asynchronous));
    } else if (v.canConvert<QQmlComponent*>()) {
        component_.reset(v.value<QQmlComponent*>());
    } else {
        component_.reset();
    }

    if (component_) {
        connect(component_.get(), &QQmlComponent::statusChanged, this, &Screen::onComponentStatusChanged);
        if (component_->isReady()) {
            onComponentStatusChanged(QQmlComponent::Ready);
        }
    }

    emit sourceChanged(v);
}

void Screen::onComponentStatusChanged(QQmlComponent::Status st) {
    if (st == QQmlComponent::Ready) {
        auto object = component_->create();
        if (object != nullptr) {
            root_item_ = qobject_cast<QQuickItem*>(object);
        }
    }
    emit statusChanged(st);
}

QDataStream& operator<<(QDataStream& s, const Screen& v) {
    s << static_cast<const Transformable&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Screen& v) {
    s >> static_cast<Transformable&>(v);
    // TODO
    return s;
}

VOLCANO_WORLD_END
